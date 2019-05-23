//
// Created by Daniel on 01.05.2019.
//

#ifndef BOOKSTORE_CUSTOMER_H
#define BOOKSTORE_CUSTOMER_H

#include <mutex>
#include <vector>
#include <string>
#include "Utilities/Book.h"
#include "Shop.h"

using MutexType = std::mutex;
using WriteLock = std::unique_lock<MutexType>;

typedef enum states {
    InQueue, Wait, Asking, Reading, Leaving
} c_states;

class Shop;

class Customer {



public:

    Customer(Shop *shop, std::string interestGenre, int nb_books, int ids[]);

    int visit_shop();

    void ask_book();

    int return_book();

    void read_book();

    int quit_shop();

    bool i_will_be_back();

    bool is_asking();

    void in_queue();

    void leave();

    Id_t get_id() const;

    const std::string &get_interested_genre() const;


    MutexType lck_custom;




    std::vector<Id_t> m_demands;

    std::vector<Book> m_my_books;
    std::vector<Book> m_read;

    bool m_new_books;
    bool m_return_book;






private:

    static Id_t c_id;
    Id_t m_id;

    Shop *m_shop;
    Library *m_lib;

    c_states m_state;

    std::string m_genre_request;

    int nb_books2ask;


    std::vector<Id_t> m_Id_requests;






    id_t last_read;

    //void init_request(int nb_books);
    void update_requests();
};

#endif //BOOKSTORE_CUSTOMER_H
