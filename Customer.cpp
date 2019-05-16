#include <utility>

//
// Created by Daniel on 01.05.2019.
//
#include <iostream>
#include <unistd.h>
#include "Customer.h"

using namespace std;

Id_t Customer::c_id = 0;

Customer::Customer(Shop *shop, string interestGenre, int nb_books, int ids[]) {

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

    for (int i = 0; i < 2; ++i) {
        m_Id_requests.push_back(ids[i]);
    }



    //init_requests(nb_books)

}

int Customer::visit_shop() {

    if (m_state != Wait)return -1;//Must be waiting to enter in the shop.

    if (m_shop->add_customer(this) == 0) {//The shop is locked internally.

        WriteLock shop_lock(m_shop->lck_shop);
        m_shop->cv_added.wait(shop_lock, [this] { return m_state == InQueue; });//Wait shop confirmation.

        std::cout << "Customer[" << m_id << "] visits shop." << std::endl;

        shop_lock.unlock();

        return 0;
    }
    return -1;//Informs main that visit failed.
}

bool Customer::ask_book() {

    bool reAsk = false;//In case he has to re-ask some books

    for (const auto &request : m_Id_requests) {//for each request given at the
        //customer construction

        if (m_lib->book_exists(m_genre_request, request)) {//If the books exists,

            m_demands.push_back(request);//then it's a demand.

        } else {
            reAsk = true;
            cerr << "You are asking for inexistent book. Id : "
                 << request << endl;
        }
    }

    WriteLock custom_lock(lck_custom);//lock the customer because of the state shared variable
    m_state = Asking;//change he's state.
    custom_lock.unlock();//Manual unlocking is done before notifying, to avoid waking up

    m_shop->seller.notify();//notify the seller that
    //he has some books demands

    WriteLock shop_lock(m_shop->lck_shop);//Waits for the seller.
    m_shop->cv_custom.wait(shop_lock, [this] { return m_new_books; });//bool flag.

    std::cout << "Customer[" << m_id << "] successfully get new books." << std::endl;
    shop_lock.unlock();//can be done just after the wait. (just for terminal print)

    m_state = Reading;
    m_new_books = false;

    return reAsk;//Informs the main that he has to re-ask or not books.
}

void Customer::read_book() {

    if (m_state == Reading) {

        //Move all the books in demands tab to the read one.
        m_read.insert(m_read.end(), std::make_move_iterator(m_my_books.begin()),
                      std::make_move_iterator(m_my_books.end()));
        m_my_books.erase(m_my_books.begin(), m_my_books.end());

        usleep(10000);//He reads really quickly

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

        m_shop->return_seller.notify();//Notify the seller

        WriteLock shop_lock(m_shop->lck_shop);//And wait that the seller
        m_shop->cv_return_custom.wait(shop_lock, [this] { return !m_return_book; });//takes back the
        //books. The seller informs using the bool m_return_book flag that the
        //customer has returned all the books.

        std::cout << "Customer[" << m_id << "] successfully returned books." << std::endl;
        shop_lock.unlock();
        if (m_Id_requests.empty()) {//Informs the main if,
            return 0;//yes : he has finish to ask, read and return books.
        }
    }
    return -1;//or not.
}



