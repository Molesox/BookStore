//
// Created by Daniel on 13.05.2019.
//

#ifndef BOOKSTORE_SHOP_H
#define BOOKSTORE_SHOP_H

#include <vector>
#include <condition_variable>
#include "Utilities/Library.h"
#include "Customer.h"
#include "Utilities/b_semaphore.h"


using MutexType = std::mutex;
using WriteLock = std::unique_lock<MutexType>;
using CV = std::condition_variable;
using semaphore = b_semaphore;

class Customer;

class Shop {
public:

    Shop(Library *lib, size_t nb_places, size_t max_books);

    ~Shop();

    int add_customer(Customer *c);

    int remove_customer(Customer *c);

    bool is_empty();

    std::vector<Customer *> m_customers;

    Library *m_lib;

    size_t m_nb_place;
    size_t m_max_books;

    MutexType lck_shop;

    CV cv_added;
    CV cv_quit;

    CV cv_custom;
    CV cv_return_custom;

    semaphore *seller;
    semaphore *return_seller;


};


#endif //BOOKSTORE_SHOP_H
