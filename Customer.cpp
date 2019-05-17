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
    return -1;
}

bool Customer::i_will_be_back() {

    WriteLock shop_lock(m_shop->lck_shop);
    m_shop->cv_quit.wait(shop_lock);//Wait shop confirmation.
    std::cout << "Customer[" << m_id << "] try again to visit shop." << std::endl;
    shop_lock.unlock();
    return true;
}


bool Customer::ask_book() {
    WriteLock custom_lock(lck_custom);//lock the customer because of the state shared variable
    bool reAsk = true;//In case he has to re-ask some books

    for (int i = 0; i < m_Id_requests.size(); i++) {//for each request given at the
        //customer construction

        if (m_lib->book_exists(m_genre_request, m_Id_requests[i])) {//If the books exists,

            m_demands.push_back(m_Id_requests[i]);//then it's a demand.
            m_Id_requests[i] = -1;
            ++nb_books2ask;


        } else {
            // reAsk = true;
            cerr << "You are asking for inexistent book. Id : "
                 << m_Id_requests[i] << endl;
        }
    }
    vector<Id_t> temp;
    for (unsigned long m_Id_request : m_Id_requests) {
        if (m_Id_request != -1) {
            temp.push_back(m_Id_request);
        }
    }
    m_Id_requests = temp;


    m_state = Asking;//change he's state.
    custom_lock.unlock();//Manual unlocking is done before notifying, to avoid waking up

    WriteLock shop_lock(m_shop->lck_shop);//Waits for the seller.
    m_shop->seller->up();//notify the seller that
    //he has some books demands
    m_shop->cv_custom.wait(shop_lock, [&] { return m_new_books; });//bool flag.

    std::cout << "Customer[" << m_id << "] successfully get new books." << std::endl;
    shop_lock.unlock();//can be done just after the wait. (just for terminal print)

    custom_lock.lock();
    nb_books2ask -= m_my_books.size();

    if (nb_books2ask == 0) {
        reAsk = false;
    }
    m_state = Reading;
    m_new_books = false;
    custom_lock.unlock();
    return reAsk;//Informs the main that he has to re-ask or not books.
}

void Customer::read_book() {

    if (m_state == Reading) {



        //Move all the books in demands tab to the read one.
        m_read.insert(m_read.end(), std::make_move_iterator(m_my_books.begin()),
                      std::make_move_iterator(m_my_books.end()));
        m_my_books.erase(m_my_books.begin(), m_my_books.end());

        usleep(1000000);//He reads really quickly

        m_return_book = true;//He have books to return.Bool flag for Seller

        WriteLock shop_lock(m_shop->lck_shop);
        std::cout << "Customer[" << m_id << "] has read." << std::endl;
        shop_lock.unlock();

    } else {
        cerr << "Fatal error." << endl;
        exit(88);
    }

}

int Customer::return_book() {

    if (m_return_book) {//If he has books to return,
        WriteLock shop_lock(m_shop->lck_shop);//And wait that the seller
        m_shop->return_seller->up();//Notify the seller


        m_shop->cv_return_custom.wait(shop_lock, [&] { return !m_return_book; });//takes back the
        //books. The seller informs using the bool m_return_book flag that the
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
