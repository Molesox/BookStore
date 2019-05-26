//
// Created by Daniel on 13.05.2019.
//
#include <algorithm>
#include "Shop.h"

using namespace std;

Shop::Shop(Library *lib, size_t nb_places, size_t max_books) {
    m_lib = lib;
    m_nb_place = nb_places - 1;
    m_max_books = max_books;
    m_customers = std::vector<Customer *>();
    seller = new semaphore(0);
    return_seller = new semaphore(0);

}

Shop::~Shop() {
    delete seller;
    delete return_seller;

}


int Shop::add_customer(Customer *c) {

    WriteLock s_lck(lck_shop);//Locks the shop.

    if (m_nb_place < m_customers.size()) return -1;//Check if there
    // is place in the shop.

    bool found = false;
    for (auto &customer : m_customers) {//Checks if the customer is not already inside.
        if (customer->get_id() == c->get_id()) {
            found = true;
            break;
        }
    }
    if (found) return -1;//If not,

    m_customers.push_back(c);//add the customer to the shop.
    c->in_queue();

    s_lck.unlock();//frees the shop

    cv_added.notify_all();//and notify all the customers that they are inside
    //also notifies the sleeping seller.

    return 0;
}


int Shop::remove_customer(Customer *c) {


    if (is_empty()) {//Check if the shop is already empty.
        cerr << "Fatal error. Customer[" << c->get_id()
             << "] can't quit the shop because is empty" << endl;
        return -1;
    }

    WriteLock s_lck(lck_shop);//Locks the shop.

    //Find the customer in the shop.
    auto position = std::find(m_customers.begin(), m_customers.end(), c);
    if (position != m_customers.end()) {//Iterator stuff.
        (*position)->leave();//Bool flag for customer notification
        m_customers.erase(position);//Frees space
    } else {//If is not inside
        cerr << "Customer[" << c->get_id()
             << "] can't quit the shop because is not inside" << endl;
        return -1;
    }


    s_lck.unlock();//frees the shop before notification to avoid waking up.
    cv_quit.notify_all();//and notify all the waiting customers.

    return 0;
}

bool Shop::is_empty() {
    WriteLock s_lck(lck_shop);
    return m_customers.empty();
}

void Shop::close() {
    cv_added.notify_one();
}

void Shop::notify_seller() {

    seller->up();
}

void Shop::notify_return_seller() {
    return_seller->up();

}

void Shop::notify_return_customer_all() {
    cv_return_custom.notify_all();

}

size_t Shop::max_books() const {
    return m_max_books;
}
