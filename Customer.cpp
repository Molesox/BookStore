#include <random>

#include <utility>

//
// Created by Daniel on 01.05.2019.
//
#include <iostream>
#include <unistd.h>
#include <algorithm>
#include <thread>
#include "Customer.h"
#include "Utilities/Logger.h"

using namespace std;

Id_t Customer::c_id = 0;
FileLogger *Customer::logger = new FileLogger("Customers", "Customers.log");

Customer::Customer(Shop *shop, string interestGenre, int nb_books) {


    m_lib = shop->m_lib;
    m_shop = shop;
    m_genre_request = std::move(interestGenre);
    m_Id_requests = vector<Id_t>();
    m_demands = vector<Id_t>();
    m_my_books = vector<Book>();
    m_read = vector<Book>();
    m_state = Wait;
    c_id++;
    m_id = c_id;

    m_new_books = false;
    m_return_book = false;

    nb_books2ask = 0;


    init_request(nb_books);

}

int Customer::visit_shop() {

    if (m_state != Wait)return -1;//Must be waiting to enter in the shop.

    if (m_shop->add_customer(this) == 0) {//The shop is locked internally.

        WriteLock shop_lock(m_shop->lck_shop);
        m_shop->cv_added.wait(shop_lock, [&] { return m_state == InQueue; });//Wait shop confirmation.

        std::cout << "Customer[" << m_id << "] visits shop." << std::endl;
        logger->log("Customer[" + to_string(m_id) + "] visits shop. He's interested in " + m_genre_request);
        shop_lock.unlock();

        return 0;
    }

    std::cout << "Customer[" << m_id << "] can't enter, no place... sorry." << std::endl;
    logger->log("Customer[" + to_string(m_id) + "] can't enter shop. Waiting to retry.");
    return -1;//Informs main that visit failed.
}

int Customer::quit_shop() {

    if (m_shop->remove_customer(this) == 0) {//The shop is locked internally.

        WriteLock shop_lock(m_shop->lck_shop);
        m_shop->cv_quit.wait(shop_lock, [&] { return m_state == Leaving; });//Wait shop confirmation.

        std::cout << "Customer[" << m_id << "] quit shop." << endl;
        logger->log("Customer[" + to_string(m_id) + "] quit shop.");
        shop_lock.unlock();

        return 0;
    }
    //This would be a huge error.
    return -1;
}

bool Customer::i_will_be_back() {

    WriteLock shop_lock(m_shop->lck_shop);
    m_shop->cv_quit.wait(shop_lock);//Wait for some customer leaving the shop.

    std::cout << "Customer[" << m_id << "] tries again to visit shop." << std::endl;
    logger->log("Customer[" + to_string(m_id) + "] tries again to visit shop. ");
    shop_lock.unlock();

    return true;
}

void Customer::ask_book() {

    if (m_state == InQueue) {


        for (unsigned long &m_Id_request : m_Id_requests) {//For each request given at the
            //customer construction

            if (m_lib->book_exists(m_genre_request, m_Id_request)) {//If the books exists,

                m_demands.push_back(m_Id_request);//then it's a demand.
                m_Id_request = -1;
                ++nb_books2ask;//The nb of books to ask may be different of the nb of
                //books requested at customer construction.

            } else {
                cerr << "You are asking for inexistent book. Id : "
                     << m_Id_request << endl;
                logger->log(FileLogger::e_logType::LOG_WARNING + "Customer[" + to_string(m_id)
                            + "] asking for inexistent book. Id : " + to_string(m_Id_request));
            }
        }
        update_requests();
    }

    m_state = Asking;//change he's state.
    m_shop->notify_seller();//notify the seller that he has some books demands
    WriteLock shop_lock(m_shop->lck_shop);//Waits for the seller.

    m_shop->cv_custom.wait(shop_lock, [&] { return m_new_books; });//bool flag.

    std::cout << "Customer[" << m_id << "] successfully get new books." << std::endl;
    logger->log("Customer[" + to_string(m_id) + "] successfully got new books.");
    shop_lock.unlock();//can be done just after the wait. (just for terminal print)

    nb_books2ask -= m_my_books.size();//decrements the books to ask

    m_state = Reading;//change state for read_book()
    m_new_books = false;//for the possible next call of ask_book.
    // First initialisation is in customer constructor.
}

void Customer::update_requests() {
    //TODO: This is not optimized, maybe there is a way to do it at the same time when we push the demands.
    vector<Id_t> temp;
    for (unsigned long m_Id_request : m_Id_requests) {
        if (m_Id_request != -1) {
            temp.push_back(m_Id_request);
        }
    }
    m_Id_requests = temp;
}

void Customer::read_book() {

    if (m_state == Reading) {

        //Move all the books in demands tab to the read one.
        m_read.insert(m_read.end(), std::make_move_iterator(m_my_books.begin()),
                      std::make_move_iterator(m_my_books.end()));
        m_my_books.erase(m_my_books.begin(), m_my_books.end());

        std::this_thread::sleep_for(std::chrono::milliseconds(1000));

        m_return_book = true;//He have books to return.Bool flag for seller thread.


        //TODO:: kick this out at the end of project.
        WriteLock shop_lock(m_shop->lck_shop);//Just for homogeneous print int terminal.
        std::cout << "Customer[" << m_id << "] has read." << std::endl;
        logger->log("Customer[" + to_string(m_id) + "] is done reading.");
        shop_lock.unlock();

    } else {
        //To read we must be in read state otherwise it doesn't make sense.
        cerr << "Fatal error." << endl;
        logger->log(FileLogger::e_logType::LOG_ERROR + "Fatal error.");
        exit(88);
    }

}

int Customer::return_book() {

    if (m_return_book) {//If he has books to return,

        m_shop->notify_return_seller();//Notify the seller

        WriteLock shop_lock(m_shop->lck_shop);//And wait that the seller
        m_shop->cv_return_custom.wait(shop_lock, [&] { return !m_return_book; });//takes back the
        //books. The seller informs using the bool m_return_book flag that THIS
        //customer has returned all the books.
        std::cout << "Customer[" << m_id << "] successfully returned books." << std::endl;
        logger->log("Customer[" + to_string(m_id) + "] successfully returned books.");
        shop_lock.unlock();

        if (nb_books2ask <= 0) {//Informs the main if,
            std::cout << "Customer[" << m_id << "] successfully  returned ALL! books." << std::endl;
            logger->log("Customer[" + to_string(m_id) + "] successfully  returned ALL! books.");
            return 0;//yes : he has finish to ask, read and return books.
        }
    }
    return -1;//or not.
}

bool Customer::is_asking() {
    return m_state == Asking;
}

void Customer::in_queue() {
    m_state = InQueue;
}

void Customer::leave() {
    m_state = Leaving;

}

Id_t Customer::get_id() const {
    return m_id;
}

const string &Customer::get_interested_genre() const {
    return m_genre_request;
}


void Customer::init_request(int nb_books) {     //add nb_books random book ids to the request vector of this customer


    auto list_ids = m_lib->getIds(m_genre_request);


    if (nb_books == -1) {

        m_Id_requests = list_ids;

    } else {

        std::random_shuffle(list_ids.begin(), list_ids.end());
        std::cout << " custom id : " << m_id << "requests:  " << std::endl;
        for (int i = 0; i < nb_books && i < list_ids.size(); ++i) {
            std::cout << " id " << list_ids[i] << " title " << m_lib->get_title_by_Id(list_ids[i]) << std::endl;
            m_Id_requests.push_back(list_ids[i]);
        }

    }
}