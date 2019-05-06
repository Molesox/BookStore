#include <iostream>
#include <string>
#include "Book.h"
#include "Library.h"


using namespace std;


int main() {

    Library l = Library(R"(..\book_dataset.csv)");
    cout << l;

    Id_t in;
    while (cin >> in)
        l.print_1book(in);


    return 0;
}
