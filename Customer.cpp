//
// Created by Daniel on 01.05.2019.
//

#include "Customer.h"

using namespace std;

Id_t Customer::m_id = 0;

Customer::Customer(const Library *lib, const std::string &interest_genre, u_int) {
    m_id++;
    m_lib = lib;
    m_read_books = 0;
    m_requets_books = new vector<Book *>;

}
