#include <iostream>
#include <string>
#include "Utilities/Library.h"


using namespace std;


int main() {

    Library l = Library(R"(..\book_dataset.csv)");
    cout << l;

    Id_t in;
    while (cin >> in)
        l.exist(in, "Roman");


    return 0;
}
