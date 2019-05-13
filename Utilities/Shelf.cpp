//
// Created by Daniel on 04.05.2019.
//
#include <iostream>
#include "Shelf.h"

using namespace std;

/**
 * Given  a Book pointer as argument we add it to the shelf.
 * There is no check for the Id. If there is another book in the shelf
 * with the same Id it will be removed.
 * @param pointer to the book to add.
 */
void Shelf::add_book(Book *book) {

    WriteLock lock1(m_mutex);
    m_shelf[book->get_id()] = book;
}

/**
 * Returns a pointer to the book we want. If the book is not available
 * nullptr is return. If its available it marks as not anymore.
 * The number of reads of the book is increase by one.
 * @param id of the book we want.
 * @return pointer to the book or nullptr if is not available or inexistent.
 */
Book *Shelf::borrow(Id_t id) {

    WriteLock lock1(m_mutex);

    if (m_shelf.find(id) == m_shelf.end()) {
        cout << "The book with id: " << id << " not found." << endl;
        return nullptr;
    }

    Book *b = m_shelf[id];

    if (b->is_borrowed()) {

        cout << "Unable to access book" << endl;
        return nullptr;
    }
    b->borrow();

    return b;
}

const bool Shelf::book_exists(const Id_t id) const{
    ReadLock lock1(m_mutex);

    if (m_shelf.find(id) == m_shelf.end()) {
        cerr << "The book with id " << id << " does not exist in this shelf." << endl;
        return false;
    }
    return true;
}

Shelf::Shelf(Shelf &&shelf) noexcept {

    WriteLock rhs_lk(shelf.m_mutex);

    m_genre = std::move(shelf.m_genre);
    m_shelf = std::move(shelf.m_shelf);

}

/*
 * We must use std::lock(m1, m2) to lock the two mutexes,
 * instead of just locking them one after the other.
 * If we lock them one after the other, then when two threads
 * assign two objects in opposite order like :
 *  Thread 1
 *      x = std::move(y);
 *  Thread 2
 *      y = std::move(x);
 * we could get a deadlock.
 * Even if there is no reason to do so. This constructor must be
 * explicit if we wanna put shelf() in a std::vector<>
 */
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

/**
 * A shelf is mainly defined by the genre.
 * @return the genre
 */
const std::string Shelf::getMGenre() const {
    //TODO:: Should we put a readlock here?
    //I don't think is necessary because genre should not change after
    //initialisation.
    return m_genre;
}

/**
 * Should be used only in the library creation.
 * @param mGenre, a string.
 */
void Shelf::setMGenre(const std::string &mGenre) {
    WriteLock lock1(m_mutex);
    m_genre = mGenre;
}

/**
 * This method is const, you cannot modify books inside.
 * @return const reference to the unordered_map<Id, Book*> of the shelf.
 */
const hash_map_books_t &Shelf::getMShelf() const {
    ReadLock lock1(m_mutex);
    return m_shelf;
}

Shelf::~Shelf() {
    //TODO: Should be the delete part in the book destructor instead ?
    for (auto b : m_shelf) {
        delete b.second;
    }
    m_shelf.clear();
}

/**
 * Number of books in the shelf.
 * @return size_t
 */
const size_t Shelf::nb_books() const {
    ReadLock lock1(m_mutex);
    return m_shelf.size();
}

