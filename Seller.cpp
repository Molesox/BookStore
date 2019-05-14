//
// Created by Daniel on 01.05.2019.
//

#include "Seller.h"

using namespace std;

void Seller::give_book() {
    WriteLock s_lock(m_shop->lck_shop);
    m_shop->cv_return_seller.wait(s_lock);

    for (auto c : m_shop->m_customers) {
        if (try_lock(c->lck_custom)) {
            for (int i = 0; i < m_shop->m_max_books; ++i) {
                if (m_shop->m_lib->book_exists(c->m_genre_request, c->m_demands[i])) {
                    c->m_my_books.push_back(m_shop->m_lib.)
                }

            }
        }
    }
}

void Seller::get_back_book() {

}
