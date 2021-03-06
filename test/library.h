//
// Created by Daniel on 25.04.2019.
//

#pragma clang diagnostic push
#pragma ide diagnostic ignored "OCUnusedGlobalDeclarationInspection"

#ifndef BOOKSTORE_LIBRARY_H
#define BOOKSTORE_LIBRARY_H

#include <vector>
#include <iostream>
#include <fstream>
#include <string>
#include <queue>

#include "Shelf.h"


typedef std::ifstream File;
typedef std::vector<Shelf> library_t;

class Library {

public:

    //CONSTRUCTORS & DESTRUCTORS

    explicit Library(std::string const &filename);

    Library(const Library &lib);


    //METHODS

    void print(std::ostream &flux) const;

    void init_max_shelf();

    void print_genres_occurences() const;

    void print_1book(Id_t id) const;

    Book *extract_book(const std::string &line);

    Book borrow(Id_t id, const std::string &genre);

    void unborrow(const Book &b);

    const bool book_exists(const std::string &genre, Id_t id) const;

    //GETTERS && SETTERS

    size_t getNbShelfs() const;

    size_t getNbBooks() const;

    size_t getBiggestShelf() const;

    Shelf get_shelf_by_genre(std::string genre);



    //ATTRIBUTES

private:

    std::string m_filename;
    File m_dataset;//make it a pointer ? Is ifstream the best type?

    library_t m_library;

    size_t m_nb_shelfs;//same as nb of genres
    size_t m_nb_books;
    size_t m_max_shelf;

    std::string m_max_genre;


};

std::ostream &operator<<(std::ostream &os, const Library &library);

#endif //BOOKSTORE_LIBRARY_H

#pragma clang diagnostic pop
