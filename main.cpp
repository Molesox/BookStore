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
            std::cout << " john leaves " << std::endl;
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


    char in = 'n';
    while (in != 'q') {

        cout << "Hi, select scenario 1, 2, 3 or 'q' to quit" << endl;
        cin >> in;

        if (in == '1') {
            Shop *migros = new Shop(l, 2, 1);
            logger->log("Initialized the shop. Scenario 1");

            Seller john(migros);
            thread t_john(john_thread, &john);

            Customer Samantha(migros, "Treatise", ALL);
            Customer Kimberly(migros, "Time travel", ALL);

            thread t_sam(custom_thread, &Samantha);
            thread t_kim(custom_thread, &Kimberly);

            t_sam.join();
            t_kim.join();

            john.quit();
            migros->close();

            t_john.join();

            delete migros;

        } else if (in == '2') {
            Shop *migros = new Shop(l, 4, 2);
            logger->log("Initialized the shop. Scenario 2");

            Seller john(migros);
            thread t_john(john_thread, &john);

            Customer Samantha(migros, "Treatise", 4);
            Customer Kimberly(migros, "Treatise", 6);
            Customer Tungstene(migros, "Treatise", 7);
            Customer Anne(migros, "Treatise", 6);

            thread t_sam(custom_thread, &Samantha);
            thread t_kim(custom_thread, &Kimberly);
            thread t_tun(custom_thread, &Tungstene);
            thread t_ann(custom_thread, &Anne);

            t_sam.join();
            t_kim.join();

            t_tun.join();
            t_ann.join();

            cout << "chichi" << endl;
            john.quit();
            migros->close();

            t_john.join();

            delete migros;
        }


    }

    delete l;

    logger->log("Program terrminated.");
    delete logger;
    return 0;
}
