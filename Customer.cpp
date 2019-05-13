//
// Created by Daniel on 01.05.2019.
//
#include <iostream>
#include <unistd.h>
#include "Customer.h"

using namespace std;

int Customer::visit_shop() {

    if (m_state != Wait)return -1;

    if (m_shop->add_customer(this) != -1) {

        WriteLock s_lck(m_shop->lck_shop);
        m_shop->cv_added.wait(s_lck, m_state == InQueue);
        return 0;
    }
    return -1;
}

bool Customer::ask_book() {

    WriteLock custom_lock(lck_custom);
    bool reAsk = false;

    for (const auto &request : m_Id_requests) {
        if (m_lib->book_exist(m_genre_request, request)) {
            m_demands.push_back(request);
        } else {
            reAsk = true;
            cerr << "You are asking for inexistent book. Id : "
                 << request << endl;
        }
    }
    m_state = Asking;
    custom_lock.unlock();

    m_shop->cv_seller.notify_one();

    custom_lock.lock();


    {
        WriteLock s_lck(m_shop->lck_shop);
        m_shop->cv_custom.wait(s_lck, m_return_book);
    }
    m_state = Reading;
    m_new_books = false;
    return reAsk;
}

int Customer::return_book() {

    WriteLock custom_lock(lck_custom);

    if (m_return_book) {

        m_shop->cv_return_seller.notify_one();
        WriteLock shop_lock(m_shop->lck_shop);
        m_shop->cv_return_custom.wait(shop_lock, !m_return_book);

        if (m_Id_requests.empty()) {
            return 0;
        }

    }
    return -1;
}

void Customer::read_book() {
    WriteLock custom_lock(lck_custom);
    if (m_state == Reading) {
        m_read.insert(m_read.end(), std::make_move_iterator(m_my_books.begin()),
                      std::make_move_iterator(m_my_books.end()));
        m_my_books.erase(m_my_books.begin(), m_my_books.end());

        usleep(1000);
        m_return_book = true;
    }
}
