//
// Created by Daniel on 01.05.2019.
//

#ifndef BOOKSTORE_SELLER_H
#define BOOKSTORE_SELLER_H


#include "Customer.h"
#include "Logger.h"

class Customer;

typedef enum seller_states {
    Sleep, Giving, Returning, Quit
} s_states;

class Seller {

public:

    Seller(Shop *shop);

    int give_book();

    void get_back_book();

    void quit();

private:
    Shop *m_shop;
    s_states m_state;
    FileLogger *logger = new FileLogger("John", "John.log");
};


#endif //BOOKSTORE_SELLER_H
