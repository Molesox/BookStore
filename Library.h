//
// Created by Daniel on 25.04.2019.
//

#ifndef BOOKSTORE_LIBRARY_H
#define BOOKSTORE_LIBRARY_H

#include <vector>
#include <iostream>
#include <fstream>
#include <string>
#include <bits/stdc++.h>
#include "Book.h"


typedef std::vector<Book> shelf_t;
typedef std::vector<shelf_t> library_t;
typedef std::ifstream File;
typedef std::unordered_map<std::string, size_t> hash_map_t;

class Library {

public:

    Library(std::string const &filename);

    template<typename KeyType, typename ValueType>
    std::pair<KeyType, ValueType> get_max(const std::unordered_map<KeyType, ValueType> &x);

    void print_map();

    size_t getNbShelfs() const;

    void setNbShelfs(size_t nbShelfs);

    size_t getNbBooks() const;

    void setNbBooks(size_t nbBooks);

    size_t getBiggestShelf() const;

    void setBiggestShelf(size_t biggestShelf);

private:
    File m_dataset;
    hash_map_t m_hmap_genres;

    library_t library;

    size_t m_nb_shelfs;
    size_t m_nb_books;
    size_t m_biggest_shelf;

};


#endif //BOOKSTORE_LIBRARY_H
