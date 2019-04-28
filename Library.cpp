//
// Created by Daniel on 25.04.2019.
//

#include "Library.h"
#include <iostream>

using namespace std;

//CONSTRUCTORS

Library::Library(string const &filename) {

    m_dataset = ifstream(filename);
    m_nb_books = 0;

    if (m_dataset.is_open()) {


        int end_id, end_title, end_author, end_date, end_genre;//end positions in .csv file
        string id, title, author, date, genre;

        string line;
        while (getline(m_dataset, line)) {

            //TODO:: create function returning a pointer over a new book. code btw {...} should disappear.
            end_id = line.find(';');
            id = line.substr(0, end_id);

            end_title = line.find(';', end_id + 1);
            title = line.substr(end_id + 1, end_title - (end_id + 1));

            end_author = line.find(';', end_title + 1);
            author = line.substr(end_title + 1, end_author - (end_title + 1));

            end_date = line.find(';', end_author + 1);
            date = line.substr(end_author + 1, end_date - (end_author + 1));

            end_genre = line.find(';', end_date + 1);
            genre = line.substr(end_date + 1, end_genre - (end_date + 1));


            int idd = stol(id, nullptr, 10);

            Book *temp;
            temp = new Book(title, author, date, genre, idd);


            m_library[genre][idd] = temp;
            m_nb_books += 1;
        }
        m_dataset.close();




    } else cout << "Unable to open file : " << filename << "\n";
}

//METHODS

template<typename KeyType, typename ValueType>
pair<KeyType, ValueType>
Library::get_max(const unordered_map<KeyType, ValueType> &x) {
    using pairtype=pair<KeyType, ValueType>;
    return *max_element(x.begin(), x.end(), [](const pairtype &p1, const pairtype &p2) {
        return p1.second < p2.second;
    });
}

void Library::print_map() {

    for (const auto &p:m_library) {
        cout << p.first << endl;
        cout << " appears : " << p.second.size() << " times" << endl;
    }
}

//GETTERS & SETTERS

size_t Library::getNbShelfs() const {
    return m_nb_shelfs;
}

void Library::setNbShelfs(size_t nbShelfs) {
    m_nb_shelfs = nbShelfs;
}

size_t Library::getNbBooks() const {
    return m_nb_books;
}

void Library::setNbBooks(size_t nbBooks) {
    m_nb_books = nbBooks;
}

size_t Library::getBiggestShelf() const {
    return m_biggest_shelf;
}

void Library::setBiggestShelf(size_t biggestShelf) {
    m_biggest_shelf = biggestShelf;
}




