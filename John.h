//
// Created by Daniel on 01.05.2019.
//

#ifndef BOOKSTORE_JOHN_H
#define BOOKSTORE_JOHN_H

#include <string>
#include <vector>
#include "Customer.h"

typedef unsigned long int Id_t;

class John {


private:
    Id_t m_id;
    std::vector<Customer *> m_customers;
};


#endif //BOOKSTORE_JOHN_H
