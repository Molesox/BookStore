#include <utility>

//
// Created by Daniel on 01.05.2019.
//
#include <iostream>
#include <unistd.h>
#include <algorithm>
#include "Utilities/basic_semaphore.h"
#include "Customer.h"

using namespace std;

Id_t Customer::c_id = 0;

Customer::Customer(Shop *shop, string interestGenre, int nb_books, int ids[]) {

    last_read = -1;
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


    for (int i = 0; i < 3; ++i) {
        m_Id_requests.push_back(ids[i]);
    }

    //init_requests(nb_books)

}

int Customer::visit_shop() {

    if (m_state != Wait)return -1;//Must be waiting to enter in the shop.

    if (m_shop->add_customer(this) == 0) {//The shop is locked internally.

        WriteLock shop_lock(m_shop->lck_shop);
        m_shop->cv_added.wait(shop_lock, [&] { return m_state == InQueue; });//Wait shop confirmation.

        std::cout << "Customer[" << m_id << "] visits shop." << std::endl;
        shop_lock.unlock();

        return 0;
    }

    std::cout << "Customer[" << m_id << "] can't enter, no place... sorry." << std::endl;
    return -1;//Informs main that visit failed.
}

int Customer::quit_shop() {

    if (m_shop->remove_customer(this) == 0) {//The shop is locked internally.

        WriteLock shop_lock(m_shop->lck_shop);
        m_shop->cv_quit.wait(shop_lock, [&] { return m_state == Leaving; });//Wait shop confirmation.

        std::cout << "Customer[" << m_id << "] quit shop." << std::endl;
        shop_lock.unlock();

        return 0;
    }
    //This would be a huge error.
    return -1;
}

bool Customer::i_will_be_back() {

    WriteLock shop_lock(m_shop->lck_shop);
    m_shop->cv_quit.wait(shop_lock);//Wait for some customer leaving the shop.

    std::cout << "Customer[" << m_id << "] try again to visit shop." << std::endl;
    shop_lock.unlock();

    return true;
}

void Customer::ask_book() {


    for (unsigned long &m_Id_request : m_Id_requests) {//For each request given at the
        //customer construction

        if (m_lib->book_exists(m_genre_request, m_Id_request)) {//If the books exists,

            m_demands.push_back(m_Id_request);//then it's a demand.
            m_Id_request = -1;
            ++nb_books2ask;//The nb of books to ask may be different of rhe nb of
            //books requested at customer construction.

        } else {
            cerr << "You are asking for inexistent book. Id : "
                 << m_Id_request << endl;
        }
    }
    update_requests();

    m_state = Asking;//change he's state.
    WriteLock shop_lock(m_shop->lck_shop);//Waits for the seller.
    m_shop->seller->up();//notify the seller that he has some books demands
    m_shop->cv_custom.wait(shop_lock, [&] { return m_new_books; });//bool flag.

    std::cout << "Customer[" << m_id << "] successfully get new books." << std::endl;
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

        usleep(1000000);//He reads really quickly

        m_return_book = true;//He have books to return.Bool flag for seller thread.


        //TODO:: kick this out at the end of project.
        WriteLock shop_lock(m_shop->lck_shop);//Just for homogeneous print int terminal.
        std::cout << "Customer[" << m_id << "] has read." << std::endl;
        shop_lock.unlock();

    } else {
        //To read we must be in read state otherwise it doesn't make sens.
        cerr << "Fatal error." << endl;
        exit(88);
    }

}

int Customer::return_book() {

    if (m_return_book) {//If he has books to return,

        m_shop->return_seller->up();//Notify the seller

        WriteLock shop_lock(m_shop->lck_shop);//And wait that the seller
        m_shop->cv_return_custom.wait(shop_lock, [&] { return !m_return_book; });//takes back the
        //books. The seller informs using the bool m_return_book flag that THIS
        //customer has returned all the books.
        std::cout << "Customer[" << m_id << "] successfully returned books." << std::endl;
        shop_lock.unlock();

        if (nb_books2ask <= 0) {//Informs the main if,
            std::cout << "Customer[" << m_id << "] successfully  returned ALL! books." << std::endl;
            return 0;//yes : he has finish to ask, read and return books.
        }
    }
    return -1;//or not.
}



/*
 * TODO: EN maintenance, veuillez passer un autre jour.
void Customer::init_request(int nb_books){ //add nb_books random book ids to the request vector of this customer
    WriteLock shop_lock(m_shop->lck_shop);  //TOO : change for a readlock
    Shelf s = m_shop->m_lib->get_shelf_by_genre(m_genre_request);

    //Add nb_books, if there are less books in the shelf, add all of them anyway
    int n = nb_books;
    if(s.nb_books() < nb_books)
        n = s.nb_books();

    //retrieve book ids and add them to the list of requests
    for(int i = 0; i < n; i++){
        id_t temp = s.get_next_book_id(last_read);
        last_read = temp;
        m_Id_requests.push_back(temp);
    }
}
*/
