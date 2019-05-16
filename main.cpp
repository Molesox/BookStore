#include <iostream>
#include <string>
#include <thread>
#include <unistd.h>
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

    Shop *migros = new Shop(l, 20, 2);

    Seller john(migros);
    thread t_JOHN(john_thread, &john);

    thread c_threads[6];
    for (int i = 0; i < 6; ++i) {
        if (i == 4) {
            usleep(1000);
        }
        int a[] = {500 + i, 600 - i};
        c_threads[i] = thread(custom_thread, new Customer(migros, "unknown", 2, a));
    }


    for (auto &c_thread : c_threads) {
        c_thread.join();
    }
    t_JOHN.join();



    return 0;
}
