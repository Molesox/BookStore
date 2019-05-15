#include <utility>

//
// Created by Daniel on 01.05.2019.
//
#include <iostream>
#include <unistd.h>
#include "Customer.h"

using namespace std;

Id_t Customer::c_id = 0;

Customer::Customer(Shop *shop, string interestGenre, int nb_books, int ids[]) {

    last_read = -1;
    m_lib = shop->m_lib;
    m_shop = shop;
    m_genre_request = std::move(interestGenre);
    m_Id_requests = vector<Id_t>();
    m_demands = vector<Id_t>();
    m_my_books = vector<Book>();
    m_read = vector<Book>();
    m_state = Wait;
    c_id++;
    m_id = c_id;

    for (int i = 0; i < 2; ++i) {
        m_Id_requests.push_back(ids[i]);
    }



    //init_requests(nb_books)

}

int Customer::visit_shop() {

    if (m_state != Wait)return -1;//must be waiting to enter in the shop.

    if (m_shop->add_customer(this) == 0) {//the shop is locked internally.

        WriteLock s_lck(m_shop->lck_shop);
        m_shop->cv_added.wait(s_lck, [this] { return m_state == InQueue; });//wait for confirmation.

        std::cout << "Customer[" << m_id << "] visits shop. visit_shop()" << std::endl;

        s_lck.unlock();
        return 0;
    }
    return -1;
}

bool Customer::ask_book() {

    WriteLock custom_lock(lck_custom);//lock the customer, may be useless

    bool reAsk = false;//In case he has to re-ask some books

    for (const auto &request : m_Id_requests) {//for each request given at the
        //customer construction

        if (m_lib->book_exists(m_genre_request, request)) {//If the books exists,

            m_demands.push_back(request);//then it's a demand.

        } else {
            reAsk = true;
            cerr << "You are asking for inexistent book. Id : "
                 << request << endl;
        }
    }

    m_state = Asking;//change he's state.
    custom_lock.unlock();//Manual unlocking is done before notifying, to avoid waking up

    m_shop->seller.notify();//notify the seller that
    //he has some books demands



    WriteLock s_lck(m_shop->lck_shop);//Waits for the seller.
    m_shop->cv_custom.wait(s_lck, [this] { return m_new_books; });//bool flag.

    std::cout << "Customer[" << m_id << "] successfully get new books. ask_book()" << std::endl;
    s_lck.unlock();

    m_state = Reading;
    m_new_books = false;

    return reAsk;
}

void Customer::read_book() {

    WriteLock lock1(lck_custom);

    if (m_state == Reading) {

        //Move all the books in demands tab to read ones.
        m_read.insert(m_read.end(), std::make_move_iterator(m_my_books.begin()),
                      std::make_move_iterator(m_my_books.end()));
        m_my_books.erase(m_my_books.begin(), m_my_books.end());

        usleep(1000);//He reads really quickly

        m_return_book = true;//He have books to return.Bool flag for Seller
        std::cout << "Customer[" << m_id << "] has read. read_book()" << std::endl;

    }
    lock1.unlock();
}

int Customer::return_book() {


    lck_custom.unlock();
    if (m_return_book) {//If he has books to return,

        m_shop->return_seller.notify();//Notify the seller

        WriteLock shop_lock(m_shop->lck_shop);//And wait that the seller
        m_shop->cv_return_custom.wait(shop_lock, [this] { return !m_return_book; });//takes back the

        std::cout << "Customer[" << m_id << "] successfully returned books. return_book()" << std::endl;

        //books. The seller informs using the bool m_return_book flag that the
        //customer has returned all the books.

        if (m_Id_requests.empty()) {//Informs the main if,
            return 0;//yes : he has finish to ask, read and return books.

        }

    }
    return -1;//or not.
}

void Customer::init_request(int nb_books){ //add nb_books random book ids to the request vector of this customer
    WriteLock shop_lock(m_shop->lck_shop);  //TODO : change for a readlock
    Shelf s = m_shop->m_lib->get_shelf_by_genre(m_genre_request);

    //Add nb_books, if there are less books in the shelf, add all of them anyway
    int n = nb_books;
    if(s.nb_books() < nb_books)
        n = s.nb_books();

    //retrieve book ids and add them to the list of requests
    for(int i = 0; i < n; i++){
        id_t temp = s.get_next_book_id(last_read);
        last_read = temp;
        m_Id_requests.push_back(temp);
    }
}

