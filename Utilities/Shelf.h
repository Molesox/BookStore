//
// Created by Daniel on 04.05.2019.
//

#ifndef BOOKSTORE_SHELF_H
#define BOOKSTORE_SHELF_H

#include <mutex>
#include <unordered_map>
#include <unordered_set>
#include <vector>

#include "Book.h"

typedef std::unordered_map<Id_t, Book *> hash_map_books_t;

class Shelf {

    using MutexType = std::mutex;
    using ReadLock = std::unique_lock<MutexType>;
    using WriteLock = std::unique_lock<MutexType>;


    //ATTRIBUTES


private:
    std::vector<Id_t> m_list_ids;
    mutable MutexType m_mutex;
    std::string m_genre;
    hash_map_books_t m_shelf;


public:

    //CONSTRUCTORS & MOVE & COPY & DESTRUCTORS

    Shelf() = default;

    ~Shelf();

    Shelf(Shelf &&shelf) noexcept;

    Shelf(const Shelf &a);

    Shelf &operator=(const Shelf &a);

    Shelf &operator=(Shelf &&a) noexcept;

    //METHODS

    const size_t nb_books() const;

    void add_book(Book *book);

    Book *borrow(Id_t id);

    void unborrow(Id_t id);

    const bool book_exists(Id_t id) const;

    void add_id(Id_t t);

    std::vector<Id_t> getMListIds() const;

    //GETTERS & SETTERS

    const std::string getMGenre() const;

    void setMGenre(const std::string &mGenre);

    const hash_map_books_t &getMShelf() const;


    const bool is_borrowed(const Id_t id) const;
};


#endif //BOOKSTORE_SHELF_H
