#include <iostream>
#include <string>
#include <thread>
#include <unistd.h>
#include "Utilities/Library.h"
#include "Shop.h"
#include "Seller.h"
#include "Utilities/Logger.h"

#include <random>
#include <chrono>
#include <atomic>


using namespace std;

#define ALL -1

void custom_thread(Customer *c) {
    hello:
    if (c->visit_shop() == 0) {

        do {
            c->ask_book();
            c->read_book();
        } while (c->return_book() != 0);

        c->quit_shop();

        return;

    } else {
        c->i_will_be_back();
        goto hello;
    }

}


void john_thread(Seller *s) {

    while (true) {
        if (s->give_book() == -2) {
            break;
        }
        s->get_back_book();
    }
}

int main(int argc, char *argv[]) {

    if (argv[1] == nullptr) {
        cerr << "No library given as program argument.";
    }

    FileLogger *logger = new FileLogger("Main", "MainLog.log");

    Library *l = new Library(argv[1]);
    logger->log("Initialized library.");
    cout << *l << endl << endl;

    Shop *migros = new Shop(l, 2, 1);
    logger->log("Initialized the shop.");

    Seller john(migros);
    thread t_john(john_thread, &john);

    Customer Samantha(migros, "Treatise", ALL);
    Customer Kimberly(migros, "Time travel", ALL);

    thread t_sam(custom_thread, &Samantha);
    thread t_kim(custom_thread, &Kimberly);


    john.quit();
    migros->close();
    t_john.join();



    delete l;
    delete migros;

    char in = 'n';
    while (in != 'y') {
        std::cout << "wanna quit [y,n] ";
        cin >> in;
    }

    logger->log("Program terrminated.");
    return 0;
}
