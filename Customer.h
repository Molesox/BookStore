//
// Created by Daniel on 01.05.2019.
//

#ifndef BOOKSTORE_CUSTOMER_H
#define BOOKSTORE_CUSTOMER_H

#include <mutex>
#include <vector>
#include <string>
#include "Utilities/Book.h"

using MutexType = std::mutex;
using WriteLock = std::unique_lock<MutexType>;

class Customer {

public:

    MutexType lck_custom;

    std::string genre_request;

    std::vector<Id_t> m_Id_requests;
    std::vector<Id_t> m_demands;

    std::vector<Book> m_my_books;
    std::vector<Book> m_read;

    bool new_books;
    bool return_book;


private:
};
#endif //BOOKSTORE_CUSTOMER_H
