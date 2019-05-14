//
// Created by Daniel on 13.05.2019.
//

#include "Shop.h"


int Shop::add_customer(Customer *c) {

    WriteLock s_lck(lck_shop);
    if (m_nb_place > m_customers.size()) return -1;

    bool found = false;
    for (auto cust : m_customers) {
        if (cust->m_id == c->m_id) {
            found = true;
            break;
        }
    }
    if (!found) return -1;

    m_customers.push_back(c);
    s_lck.unlock();
    cv_added.notify_all();

    return 0;
}
