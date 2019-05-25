
//
// Created by Daniel on 24.04.2019.
//

#include <iostream>
#include <utility>

#include "Book.h"

using namespace std;
Id_t Book::id = 0;

//CONSTRUCTORS

Book::Book(std::string name, std::string author,
           std::string date, std::string genre, Id_t wiki_id, int nb_reads) {

    m_name = std::move(name);
    m_author = std::move(author);
    m_date = std::move(date);
    m_genre = std::move(genre);
    m_wiki_id = wiki_id;
    m_nb_reads = nb_reads;
    m_is_borrowed = false;
    m_id = id + 1;
    id++;
}


//METHODS

void Book::print(std::ostream &flux) const {

    flux << "Id : " << m_id << "\n";
    flux << "Name : " + m_name << "\n";
    flux << "Genre : " + m_genre << endl;
    flux << "Author : " + m_author << "\n";
    flux << "Has been read : " << m_nb_reads << " times\n";
    flux << "Date of publication : " + m_date << "\n";
    flux << "Wiki id : " << m_wiki_id << "\n";

    if (is_borrowed()) {
        flux << "The book is borrowed";
    } else {
        flux << "The book is available";
    }
    flux << "\n";
}

void Book::borrow() {
    m_is_borrowed = true;
}

void Book::unborrow() {
    m_is_borrowed = false;
}

//GETTERS AND SETTERS

unsigned long int Book::get_id() const {
    return m_id;
}

bool Book::is_borrowed() const {
    return m_is_borrowed;
}

int Book::getMNbReads() const {
    return m_nb_reads;
}

void Book::setMNbReads(int mNbReads) {
    m_nb_reads = mNbReads;
}


//SURCHARGE OF OPERATORS

ostream &operator<<(ostream &os, const Book &book) {

    book.print(os);
    return os;
}
