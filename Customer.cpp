//
// Created by Daniel on 01.05.2019.
//
#include <iostream>
#include <unistd.h>
#include "Customer.h"

using namespace std;

int Customer::visit_shop() {

    if (m_state != Wait)return -1;//must be waiting to enter in the shop.

    if (m_shop->add_customer(this) != -1) {//the shop is locked internally.

        WriteLock s_lck(m_shop->lck_shop);

        m_shop->cv_added.wait(s_lck, [this] { return m_state == InQueue; });//wait for confirmation.

        return 0;
    }
    return -1;
}

bool Customer::ask_book() {

    WriteLock custom_lock(lck_custom);//lock the customer
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
    m_state = Asking;//change he's state.
    custom_lock.unlock();//Manual unlocking is done before notifying, to avoid waking up

    m_shop->cv_seller.notify_one();//notify the seller that
    //he has some books demands

    custom_lock.lock();//may be useless...


    {
        WriteLock s_lck(m_shop->lck_shop);//Waits for the seller.
        m_shop->cv_custom.wait(s_lck, [this] { return m_new_books; });//bool flag.
    }
    m_state = Reading;
    m_new_books = false;
    return reAsk;
}

void Customer::read_book() {
    WriteLock custom_lock(lck_custom);//Lock customer. May be useless.
    if (m_state == Reading) {
        //Move all the books in demands tab to read ones.
        m_read.insert(m_read.end(), std::make_move_iterator(m_my_books.begin()),
                      std::make_move_iterator(m_my_books.end()));
        m_my_books.erase(m_my_books.begin(), m_my_books.end());

        usleep(1000);//He reads really quickly
        m_return_book = true;//He have books to return.
    }
}

int Customer::return_book() {

    WriteLock custom_lock(lck_custom);//Lock customer. May be useless.

    if (m_return_book) {//If he has books to return,

        m_shop->cv_return_seller.notify_one();//Notify the seller
        WriteLock shop_lock(m_shop->lck_shop);//And wait that the seller
        m_shop->cv_return_custom.wait(shop_lock, [this] { return !m_return_book; });//takes back the
        //books. The seller informs using the bool m_return_book flag that the
        //customer has returned all the books.

        if (m_Id_requests.empty()) {//Informs the main if,
            return 0;//yes : he has finish to ask, read and return books.
        }

    }
    return -1;//or not.
}


