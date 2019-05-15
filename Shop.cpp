//
// Created by Daniel on 13.05.2019.
//

#include "Shop.h"

using namespace std;

Shop::Shop(Library *lib, size_t nb_places, size_t max_books) {
    m_lib = lib;
    m_nb_place = nb_places;
    m_max_books = max_books;
    m_customers = std::vector<Customer *>();
}


int Shop::add_customer(Customer *c) {

    WriteLock s_lck(lck_shop);//Locks the shop.

    if (m_nb_place < m_customers.size()) return -1;//Check if there
    // is place in the shop.


    bool found = false;
    for (auto &customer : m_customers) {//Checks if the customer is not already inside.
        if (customer->m_id == c->m_id) {
            found = true;
            break;
        }
    }
    if (found) return -1;//If not,

    m_customers.push_back(c);//add the customer to the shop.
    c->m_state = InQueue;

    s_lck.unlock();//frees the shop

    cv_added.notify_all();//and notify the customer that he's inside.
    std::cout << "Customer[" << c->m_id << "] added to the shop. add_customer()" << std::endl;

    return 0;
}
