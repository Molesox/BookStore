//
// Created by Daniel on 04.05.2019.
//
#include <iostream>
#include "Shelf.h"

using namespace std;

void Shelf::add_book(Book *book) {
    //TODO::comment lock proprement?
    m_shelf[book->get_id()] = book;

}

Book *Shelf::borrow(Id_t id) {
    //TODO:: read and write lock ?
    Book *b = m_shelf[id];
    if (b->is_borrowed()) {
        cout << "unable to acces book" << endl;
        return nullptr;
    }
    b->borrow();
    return b;
}

Shelf::Shelf(Shelf &&shelf) noexcept {

    WriteLock rhs_lk(shelf.m_mutex);

    m_genre = std::move(shelf.m_genre);
    m_shelf = std::move(shelf.m_shelf);

}

Shelf &Shelf::operator=(Shelf &&a) noexcept {

    if (this != &a) {
        WriteLock lhs_lk(m_mutex, std::defer_lock);
        WriteLock rhs_lk(a.m_mutex, std::defer_lock);

        std::lock(lhs_lk, rhs_lk);

        m_genre = std::move(a.m_genre);
        m_shelf = std::move(a.m_shelf);
    }
    return *this;

}

Shelf &Shelf::operator=(const Shelf &a) {
    if (this != &a) {
        WriteLock lhs_lk(m_mutex, std::defer_lock);
        ReadLock rhs_lk(a.m_mutex, std::defer_lock);
        std::lock(lhs_lk, rhs_lk);

        m_genre = a.m_genre;
        m_shelf = a.m_shelf;
    }
    return *this;
}

Shelf::Shelf(const Shelf &a) {

    ReadLock rhs_lk(a.m_mutex);
    m_genre = a.m_genre;
    m_shelf = a.m_shelf;
}


const std::string &Shelf::getMGenre() const {
    //TODO::readlock?
    return m_genre;
}

void Shelf::setMGenre(const std::string &mGenre) {
    //TODO::write lock?
    m_genre = mGenre;
}

const hash_map_books_t &Shelf::getMShelf() const {
    //TODO::read lock?
    return m_shelf;
}

Shelf::~Shelf() {
    for (auto b : m_shelf) {
        delete b.second;
    }
    m_shelf.clear();

}

const size_t Shelf::nb_books() const {
    return m_shelf.size();
}

