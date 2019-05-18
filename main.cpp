#include <iostream>
#include <string>
#include <thread>
#include <unistd.h>
#include "Utilities/Library.h"
#include "Shop.h"
#include "Seller.h"

#include <random>
#include <chrono>


using namespace std;

void custom_thread(Customer *c) {
    hello:
    if (c->visit_shop() == 0) {

        do {
            c->ask_book();
            c->read_book();
        } while (c->return_book() != 0);

        c->quit_shop();

    } else {
        c->i_will_be_back();
        goto hello;
    }

}

#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wmissing-noreturn"

void john_thread(Seller *s) {

    while (true) {
        s->give_book();
        s->get_back_book();
    }
}

#pragma clang diagnostic pop

int main() {

    Library *l = new Library(R"(..\book_dataset.csv)");
    cout << *l << endl << endl;

    Shop *migros = new Shop(l, 2, 2);

    Seller john(migros);
    thread t_JOHN(john_thread, &john);


    vector<Customer *> customers;
    for (int j = 0; j < 3; ++j) {
        int a[] = {500 + j, 700 - j, 600 - j};
        customers.push_back(new Customer(migros, "unknown", 3, a));
    }


    std::mt19937_64 eng{std::random_device{}()};  // or seed however you want
    std::uniform_int_distribution<> dist{10, 100};


    thread c_threads[3];
    for (int i = 0; i < 3; ++i) {
        std::this_thread::sleep_for(std::chrono::milliseconds{dist(eng)});
        c_threads[i] = thread(custom_thread, customers[i]);

    }


    for (auto &c_thread : c_threads) {
        c_thread.join();
    }

    t_JOHN.join();

    for (auto c : customers) {
        delete c;
    }

    delete l;
    delete migros;

    return 0;
}
