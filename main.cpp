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

void Scenario_One(Library *l){
    Shop *Bookstore = new Shop(l, 1, 1);
    Seller john(Bookstore);
    thread t_JOHN(john_thread, &john);

    vector<Customer *> customers;
    int a[] = {500, 700, 600};
    customers.push_back(new Customer(Bookstore, "Biography", 3, a));
    int b[] = {400,300,200};
    customers.push_back(new Customer(Bookstore, "Classic", 3, b));


    std::mt19937_64 eng{std::random_device{}()};  // or seed however you want
    std::uniform_int_distribution<> dist{100, 500};
    thread c_threads[10];
    for (int i = 0; i < 2; ++i) {
        std::this_thread::sleep_for(std::chrono::milliseconds{dist(eng)});
        c_threads[i] = thread(custom_thread, customers[i]);
    }
    for (auto &c_thread : c_threads) {
        c_thread.join();
    }

    john.quit();
    Bookstore->close();
    t_JOHN.join();

    for (auto c : customers) {
        delete c;
    }
    delete l;
    delete Bookstore;
}

void Scenario_Two(Library *l){
    Shop *Bookstore = new Shop(l, 10, 2);
    Seller john(Bookstore);
    thread t_JOHN(john_thread, &john);

    vector<Customer *> customers;
    for (int j = 0; j < 10; ++j) {
        int a[] = {500 + j, 700 - j, 600 - j};
        customers.push_back(new Customer(Bookstore, "unknown", 3, a));
    }

    std::mt19937_64 eng{std::random_device{}()};  // or seed however you want
    std::uniform_int_distribution<> dist{100, 500};
    thread c_threads[10];
    for (int i = 0; i < 10; ++i) {
        std::this_thread::sleep_for(std::chrono::milliseconds{dist(eng)});
        c_threads[i] = thread(custom_thread, customers[i]);
    }
    for (auto &c_thread : c_threads) {
        c_thread.join();
    }

    john.quit();
    Bookstore->close();
    t_JOHN.join();

    for (auto c : customers) {
        delete c;
    }
    delete l;
    delete Bookstore;
}







int main(int argc, char *argv[]) {

    if(argv[1] == nullptr){
        cerr << "No library given as program argument.";
    }

    FileLogger *logger = new FileLogger("Main", "MainLog.log");

    Library *l = new Library(argv[1]);
    logger->log("Initialized library.");
    cout << *l << endl << endl;

    Shop *migros = new Shop(l, 2, 2);
    logger->log("Initialized the shop.");

    Seller john(migros);
    thread t_JOHN(john_thread, &john);


    vector<Customer *> customers;
    for (int j = 0; j < 3; ++j) {
        int a[] = {500 + j, 700 - j, 600 - j};
        customers.push_back(new Customer(migros, "unknown", 3, a));
    }


    std::mt19937_64 eng{std::random_device{}()};  // or seed however you want
    std::uniform_int_distribution<> dist{100, 1000};


    thread c_threads[3];
    for (int i = 0; i < 3; ++i) {
        std::this_thread::sleep_for(std::chrono::milliseconds{dist(eng)});
        c_threads[i] = thread(custom_thread, customers[i]);
        logger->log("Created thread for Customer[" + to_string(i) + "].");
    }


    for (auto &c_thread : c_threads) {
        c_thread.join();
    }

    john.quit();
    migros->close();
    t_JOHN.join();

//ssss

    for (auto c : customers) {
        delete c;
    }

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
