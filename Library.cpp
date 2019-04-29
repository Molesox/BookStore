//
// Created by Daniel on 25.04.2019.
//

#include "Library.h"
#include <iostream>

using namespace std;

//CONSTRUCTORS

Library::Library(string const &filename) {
    m_filename = filename;
    m_dataset = ifstream(m_filename);
    m_nb_books = 0;

    if (m_dataset.is_open()) {

        int end_id, end_title, end_author, end_date, end_genre;//end positions in .csv file
        string title, author, date, genre;

        int id;

        string line;
        while (getline(m_dataset, line)) {

            //TODO:: create function returning a pointer over a new book. code btw {...} should disappear.
            end_id = line.find(';');
            id = stol(line.substr(0, end_id), nullptr, 10);

            end_title = line.find(';', end_id + 1);
            title = line.substr(end_id + 1, end_title - (end_id + 1));

            end_author = line.find(';', end_title + 1);
            author = line.substr(end_title + 1, end_author - (end_title + 1));

            end_date = line.find(';', end_author + 1);
            date = line.substr(end_author + 1, end_date - (end_author + 1));

            end_genre = line.find(';', end_date + 1);
            genre = line.substr(end_date + 1, end_genre - (end_date + 1));


            Book *temp;//outside the while()?
            temp = new Book(title, author, date, genre, id);


            m_library[genre][temp->get_id()] = temp;
            m_nb_books += 1;
        }
        m_dataset.close();

        init_max_shelf();

        m_nb_shelfs = m_library.size();

    } else cout << "Unable to open file : " << m_filename << "\n";
}


void Library::init_max_shelf() {
    m_max_shelf = 0;
    size_t big = 0;
    size_t temp = 0;

    for (const auto &g : m_library) {
        temp = g.second.size();
        if (temp > big) {
            big = temp;
        }
    }
    m_max_shelf = big;
}

//METHODS

void Library::print(std::ostream &flux) const {
    flux << "The library loaded from : " << m_filename << endl;
    flux << "Contains:\n"
         << m_nb_books << " books \n"
         << m_nb_shelfs << " distinct genres\n" << endl;


}

template<typename KeyType, typename ValueType>
pair<KeyType, ValueType>
Library::get_max(const unordered_map<KeyType, ValueType> &x) {
    using pairtype=pair<KeyType, ValueType>;
    return *max_element(x.begin(), x.end(), [](const pairtype &p1, const pairtype &p2) {
        return p1.second < p2.second;
    });
}

void Library::print_genres_occurences() const {

    for (const auto &pair:m_library) {
        cout << pair.first << endl;
        cout << " appears : " << pair.second.size() << " times" << endl;
    }
}

void Library::print_1book(Id_t id) const {

    for (const auto &g : m_library) {
        //C plutôt pas mal ce truc hahah car
        //j'aurais du mal a dire quel est le type de retour
        //hahahhahahahhahahahahahahha
        auto temp = g.second.find(id);

        if (temp != g.second.end()) {
            cout << *(temp->second) << endl;
        }

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
    return m_max_shelf;
}

void Library::setBiggestShelf(size_t biggestShelf) {
    m_max_shelf = biggestShelf;
}


std::ostream &operator<<(std::ostream &os, const Library &library) {
    library.print(os);
    return os;
}




