//
// Created by Daniel on 25.04.2019.
//

#include "Library.h"
#include <iostream>

#pragma clang diagnostic push
#pragma ide diagnostic ignored "OCUnusedGlobalDeclarationInspection"
using namespace std;

//CONSTRUCTORS

Library::Library(string const &filename) {

    m_filename = filename;
    m_dataset = ifstream(m_filename);
    m_library = library_t(10);
    m_nb_books = 0;
    m_max_shelf = 0;

    if (m_dataset.is_open()) {


        bool added = false;
        string line;

        while (getline(m_dataset, line)) {

            Book *temp = extract_book(line);

            for (auto &i : m_library) {

                if (i.getMGenre() == temp->m_genre) {

                    i.add_book(temp);
                    added = true;
                    break;
                }
            }
            if (!added) {
                m_library.push_back(Shelf());
                m_library.back().setMGenre(temp->m_genre);
                m_library.back().add_book(temp);
            }

            added = false;
            m_nb_books += 1;
        }

        m_dataset.close();
        init_max_shelf();
        m_nb_shelfs = m_library.size();

    } else {
        cout << "Unable to open file : " << m_filename << "\n";
        exit(2);
    }

}


Library::Library(const Library &lib) {

    m_library = lib.m_library;
    m_nb_shelfs = m_library.size();
    m_filename = lib.m_filename;
    m_nb_books = lib.m_nb_books;
    m_max_shelf = lib.m_max_shelf;


}

void Library::init_max_shelf() {

    m_max_shelf = 0;
    size_t big = 0;
    size_t temp = 0;

    for (const auto &shelf : m_library) {

        temp = shelf.nb_books();

        if (temp > big) {

            big = temp;
            m_max_genre = shelf.getMGenre();
        }
    }
    m_max_shelf = big;
}

//METHODS

void Library::print(std::ostream &flux) const {
    flux << "The library loaded from : " << m_filename << endl;
    flux << "Contains:\n"
         << m_nb_books << " books \n"
         << m_nb_shelfs << " distinct genres\n"
         << m_max_shelf << " occurences of genre : "
         << m_max_genre << endl;
}


void Library::print_genres_occurences() const {

    for (const auto &pair:m_library) {
        cout << pair.getMGenre() << endl;
        cout << " appears : " << pair.getMShelf().size() << " times" << endl;
    }
}

void Library::print_1book(Id_t id) const {

    for (const auto &shelf : m_library) {

        auto temp = shelf.getMShelf().find(id);

        if (temp != shelf.getMShelf().end()) {
            cout << *(temp->second) << endl;
            return;
        }
    }
    cout << "Book not found" << endl;
}

const bool Library::book_exists(const std::string& genre, const Id_t id) const {
    string lg;
    for(const auto &s : m_library){
        lg = s.getMGenre();
        if(lg.substr(0, lg.size() - 1) == genre) {
            return s.book_exists(id);
        }
    }
    cerr << "Genre " << genre << " not found." << endl;
    return false;
}

//GETTERS & SETTERS

size_t Library::getNbShelfs() const {
    return m_nb_shelfs;
}


size_t Library::getNbBooks() const {
    return m_nb_books;
}


size_t Library::getBiggestShelf() const {
    return m_max_shelf;
}


Book *Library::extract_book(const std::string &line) {
    static int end_id, end_title, end_author, end_date, end_genre;//end positions in .csv file
    static string title, author, date, genre;
    static int id;

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

    return new Book(title, author, date, genre, id);

}

Book Library::borrow(Id_t id, std::string genre) {
    string lg;
    for (auto &s : m_library) {
        lg = s.getMGenre();
        if (lg.substr(0, lg.size() - 1) == genre) {
            return s.borrow(id);
        }
    }
}


std::ostream &operator<<(std::ostream &os, const Library &library) {
    library.print(os);
    return os;
}

#pragma clang diagnostic pop