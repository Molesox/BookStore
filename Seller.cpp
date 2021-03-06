//
// Created by Daniel on 01.05.2019.
//

#include "Seller.h"
#include "Utilities/Logger.h"
#include <chrono>

using namespace std;

Seller::Seller(Shop *shop) : m_shop(shop), m_state(Sleep) {}

/**
 * Give a requested book to a customer
 * @return -2 if quitting, -1 if the semaphore couldn't be downed, 0 otherwise
 */
int Seller::give_book() {

    if (m_state == Sleep) {

        WriteLock shop_lock(m_shop->lck_shop);//Seller wait for some customers
        cout << "John sleeps" << endl;
        logger->log("John goes to sleep");
        m_shop->cv_added.wait(shop_lock);//If he's notified,

        if(m_state==Quit){
            return -2;
        }

        m_state = Giving;//changes his state.
        cout << "John wakes up" << endl;
        logger->log("John wakes up");
        shop_lock.unlock();//Should be useless but just in case.
    }


    if (not m_shop->seller->try_down()) {//Semaphore to know if there
        // is someone waiting for books

        return -1;//If not, return to main.
    } else {
        m_shop->seller->up();//else try_down downs if try is successful
        // so reset to the right value because no book has been given yet.
    }


    WriteLock shop_lock(m_shop->lck_shop);//Shared resource of the store.

    for (auto c : m_shop->m_customers) {//For each customer
        bool control = false;
        WriteLock custom_lock(c->lck_custom, try_to_lock);//We try to lock him
        if (custom_lock.owns_lock()) {//if the lock is successful,
            if (c->is_asking()) {//and is asking for books,

                for (int i = 0; i < m_shop->max_books(); ++i) {//we give him the max possible nb of books,
                    if (m_shop->m_lib->book_exists(c->get_interested_genre(), c->m_demands[i])) {//only if they exist.
                        if (not m_shop->m_lib->is_borrowed(c->get_interested_genre(),
                                                           c->m_demands[i])) {//and if they are not borrowed
                            c->m_my_books.push_back(m_shop->m_lib->borrow(c->m_demands[i], c->get_interested_genre()));
                            c->m_new_books = true;//bool flag to notify the customer thread that he
                            // has new books.
                            logger->log("Gave book \"" + m_shop->m_lib->get_title_by_Id(c->m_demands[i]) + "\" to Customer[" + to_string(c->get_id()) + "].");

                        } else {
                            control = true;
                        }

                    }
                }
                if (c->m_new_books && control) {//For each given book we down the count of the semaphore.
                    m_shop->seller->down();
                }
            }
            custom_lock.unlock();
        }
    }
    shop_lock.unlock();

    m_shop->cv_custom.notify_all();//We notify all the customers waiting for books.
    return 0;
}

/**
 * Get back a book from a customer
 */
void Seller::get_back_book() {

    bool done = false;//Bool condition for send notification.

    if (m_shop->is_empty()) {
        m_state = Sleep;//In give_book() if state is sleep,
        //it makes the thread wait.
        return;//no people no work.
    }

    if (not m_shop->return_seller->try_down()) {//Same mechanism as for
        //give_books but for returning this time.
        return;
    } else {
        m_shop->notify_return_seller();
    }


    WriteLock shop_lock(m_shop->lck_shop);//We lock the shop to access customers
    for (auto &c : m_shop->m_customers) {//For each customer in the shop,

        WriteLock custom_lock(c->lck_custom, try_to_lock);//We try to lock him,

        if (custom_lock.owns_lock()) {//If we own the lock,

            if (c->m_return_book) {//We check that he has some books to return.
                done = true;
                /*
                we need to set the bool condition at least once to notify the customers
                even if the fact that this thread runs means that at least one
                customer asked for it.
                But it's possible that this one is the only one and sadly can't be locked.
                 */
                m_shop->return_seller->down();//downs return books semaphore.

                for (const auto &b: c->m_read) {//for each book that has been read,
                    m_shop->m_lib->unborrow(b);//we give it back to the library.
                }
                c->m_return_book = false;//bool flag to notify the customer
                // that the books have been returned.
            }
            custom_lock.unlock();
        }
    }

    shop_lock.unlock();

    if (done) {
        m_shop->notify_return_customer_all();//Notify all the waiting customers.
        //That they hav returned books. (Or not, it depends of the bool flag for each
        //customer.
        logger->log("Some books have been returned.");
    }

}

void Seller::quit() {
    m_state = Quit;
}
