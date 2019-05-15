#include <iostream>
#include <string>
#include <thread>
#include "Utilities/Library.h"
#include "Shop.h"
#include "Seller.h"


using namespace std;

void custom_thread(Customer *c) {
    c->visit_shop();
    c->ask_book();

    c->read_book();
    c->return_book();
}

void john_thread(Seller *s) {
    s->give_book();
    s->get_back_book();
}

int main() {

    Library *l = new Library(R"(..\book_dataset.csv)");
    cout << *l;

    Shop *migros = new Shop(l, 2, 3);

    Customer one(migros, "unknown", 2);
    Seller john(migros);
    thread CACA(john_thread, &john);
    thread CUCU(custom_thread, &one);


    CUCU.join();
    CACA.join();

    return 0;
}
