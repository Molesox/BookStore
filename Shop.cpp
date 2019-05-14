//
// Created by Daniel on 13.05.2019.
//

#include "Shop.h"


int Shop::add_customer(Customer *c) {

    WriteLock s_lck(lck_shop);//Locks the shop.
    if (m_nb_place > m_customers.size()) return -1;//Check if there
    // is place in the shop.

    bool found = false;
    for (auto cust : m_customers) {//Checks if the customer is not already inside.
        if (cust->m_id == c->m_id) {
            found = true;
            break;
        }
    }
    if (!found) return -1;//If not,

    m_customers.push_back(c);//add the customer to the shop.
    s_lck.unlock();//frees the shop
    cv_added.notify_all();//and notify the customer that he's inside.

    return 0;
}
