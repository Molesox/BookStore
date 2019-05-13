//
// Created by Daniel on 13.05.2019.
//

#ifndef BOOKSTORE_SHOP_H
#define BOOKSTORE_SHOP_H

#include <vector>
#include <condition_variable>
#include "Customer.h"

using MutexType = std::mutex;
using WriteLock = std::unique_lock<MutexType>;
using CV = std::condition_variable;

class Shop {
public:

    int add_customer(Customer *c);


    std::vector<Customer *> m_customers;
    size_t nb_place;

    MutexType lck_shop;

    CV cv_added;
    CV cv_custom;
    CV cv_seller;
    CV cv_return_custom;
    CV cv_return_seller;


};


#endif //BOOKSTORE_SHOP_H
