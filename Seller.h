//
// Created by Daniel on 01.05.2019.
//

#ifndef BOOKSTORE_SELLER_H
#define BOOKSTORE_SELLER_H


#include "Customer.h"

class Customer;

class Seller {

public:

    Seller(Shop *shop);

    void give_book();

    void get_back_book();


    Shop *m_shop;
private:
};


#endif //BOOKSTORE_SELLER_H
