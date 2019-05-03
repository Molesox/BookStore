//
// Created by Daniel on 25.04.2019.
//

#ifndef BOOKSTORE_LIBRARY_H
#define BOOKSTORE_LIBRARY_H

#include <vector>
#include <iostream>
#include <fstream>
#include <string>
#include <mutex>
#include <bits/stdc++.h>
#include "Book.h"


typedef std::ifstream File;
typedef std::unordered_map<Id_t, Book *> hash_map_books_t;
//typedef std::unordered_map<std::string, hash_map_books_t> library_t;

typedef struct s_shelf {
    std::string genre;
    hash_map_books_t shelf;

} shelf_t;

typedef std::vector<shelf_t> library_t;

class Library {

public:

    //CONSTRUCTORS & DESTRUCTORS

    Library(std::string const &filename);

    Library(const Library &lib);

    ~Library(void);

    //METHODS

    void print(std::ostream &flux) const;

    template<typename KeyType, typename ValueType>
    std::pair<KeyType, ValueType> get_max(const std::unordered_map<KeyType, ValueType> &x);

    void init_max_shelf();

    void print_genres_occurences() const;

    void print_1book(Id_t id) const;

    //GETTERS & SETTERS

    size_t getNbShelfs() const;

    void setNbShelfs(size_t nbShelfs);

    size_t getNbBooks() const;

    void setNbBooks(size_t nbBooks);

    size_t getBiggestShelf() const;

    void setBiggestShelf(size_t biggestShelf);

    //ATTRIBUTES

private:

    std::string m_filename;
    File m_dataset;//make it a pointer ? Is ifstream the best type?

    library_t m_library;

    size_t m_nb_shelfs;//same as nb of genres
    size_t m_nb_books;
    size_t m_max_shelf;


};

std::ostream &operator<<(std::ostream &os, const Library &library);

#endif //BOOKSTORE_LIBRARY_H
