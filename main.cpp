#include <iostream>
#include <string>
#include <fstream>
#include "Book.h"
#include "Library.h"
#include <bits/stdc++.h>

using namespace std;


int main() {

    //TODO:: find how to use relative path instead.
    Library l = Library(R"(C:\Users\Daniel\Documents\OS\BookStore\book_dataset.csv)");
    l.print_genres_occurences();

    return 0;
}
