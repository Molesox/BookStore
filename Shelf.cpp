//
// Created by Daniel on 04.05.2019.
//

#include "Shelf.h"

void Shelf::add_book(Book *book) {
    m_mutex.lock();
    m_shelf[book->get_id()] = book;
    m_mutex.unlock();
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
    return m_genre;
}

void Shelf::setMGenre(const std::string &mGenre) {
    m_genre = mGenre;
}

const hash_map_books_t &Shelf::getMShelf() const {
    return m_shelf;
}

Shelf::~Shelf() {
    for (auto b : m_shelf) {
        delete b.second;
    }
    m_shelf.clear();

}

