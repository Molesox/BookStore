//
// Created by Daniel on 01.05.2019.
//

#ifndef BOOKSTORE_CUSTOMER_H
#define BOOKSTORE_CUSTOMER_H

#include <vector>
#include <string>
#include "Book.h"
#include "Library.h"

typedef unsigned long int Id_t;


class Customer {
    Customer(const Library *lib, const std::string &interest_genre, u_int quantity);

private:
    static Id_t m_id;
    const Library *m_lib;//Customer isn't allowed to modify the lib.
    unsigned int m_read_books;
    std::vector<Book> m_to_read;
    std::vector<Book *> *m_requets_books;
};


#endif //BOOKSTORE_CUSTOMER_H
