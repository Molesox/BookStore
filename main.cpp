#include <iostream>
#include <string>
#include "Book.h"
#include "Library.h"


using namespace std;


int main() {
//
//    //TODO:: find how to use relative path instead.
    Library l = Library(R"(C:\Users\Daniel\Documents\OS\BookStore\book_dataset.csv)");
    cout << l;
    Id_t in;
    while (cin >> in) {
        l.print_1book(in);
    }
//    vector<int> vec{1,1,3,4,5,6,7};
//
//    for (int i = 0; i < vec.size(); ++i) {
//        if (vec[i] == i) {
//            vec[i]=i*2;
//
//        } else {
//            vec.push_back(i);
//        }
//        if(i>10) break;
//    }
//    for (int j = 0; j < vec.size(); ++j) {
//        cout<<vec[j]<<endl;
//    }

    return 0;
}
