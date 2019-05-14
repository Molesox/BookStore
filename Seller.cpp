//
// Created by Daniel on 01.05.2019.
//

#include "Seller.h"

using namespace std;

void Seller::give_book() {
    WriteLock s_lock(m_shop->lck_shop);
    m_shop->cv_return_seller.wait(s_lock);

    for (auto c : m_shop->m_customers) {//for each customer
        if (c->lck_custom.try_lock()) {//if we can lock him
            for (int i = 0; i < m_shop->m_max_books; ++i) {//we give him the max possible nb of books
                if (m_shop->m_lib->book_exists(c->m_genre_request, c->m_demands[i])) {//only if they exist

                    c->m_my_books.push_back(m_shop->m_lib->borrow(c->m_demands[i], c->m_genre_request));
                    c->m_new_books = true;//bool flag to notify the customer thread that he
                    // must return the books some day.
                }

            }
        }
    }
    s_lock.unlock();
    m_shop->cv_custom.notify_all();//we notify all the customers waiting for books.
}

void Seller::get_back_book() {
    bool done = false;

    WriteLock s_lock(m_shop->lck_shop);//We lock the shop to access customers
    m_shop->cv_return_seller.wait(s_lock);//We wait for returns from customers

    for (auto &c : m_shop->m_customers) {//For each customer in the shop,
        if (c->lck_custom.try_lock()) {//We try to lock him. If yes,
            if (c->m_return_book) {//We check that he has some books to return.
                done = true;//we need to do it at least once to notify the customers
                //even if the fact that this thread runs means that at least one
                // customer ask for it.
                //But it's possible that this one can't be locked.
                for (const auto &b: c->m_read) {//for each book that has been read,
                    //TODO: Implement the unborrow function in library and in shelf.
                    m_shop->m_lib->unborrow(b);//we give it back to the library.
                }
                c->m_return_book = false;//bool flag to notify the customer
                // that the books have been returned.
            }
        }
    }
    if (done) {
        m_shop->cv_return_custom.notify_all();//notify the waiting customers.
    }

}
