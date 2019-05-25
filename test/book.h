//
// Created by Daniel on 24.04.2019.
//

#ifndef BOOKSTORE_BOOK_H
#define BOOKSTORE_BOOK_H

#include <string>
#include <vector>


typedef unsigned long int Id_t;


class Book {
public:

//CONSTRUCTORS

    Book(std::string name, std::string author, std::string date,
         std::string genre, Id_t wiki_id, int nb_reads = 0);

//METHODS

    void print(std::ostream &flux) const;

    void borrow();

    void unborrow();

//GETTERS AND SETTERS

    Id_t get_id() const;

    bool is_borrowed() const;

    int getMNbReads() const;

    void setMNbReads(int mNbReads);

//ATTRIBUTES

public:

    std::string m_name;
    std::string m_author;
    std::string m_date;
    std::string m_genre;

    static Id_t id; //Initialised in cpp file to 0


private:
    int m_nb_reads;
    bool m_is_borrowed;
    Id_t m_id;
    Id_t m_wiki_id;

};

//SURCHARGE OF OPERATORS

std::ostream &operator<<(std::ostream &os, const Book &book);

#endif //BOOKSTORE_BOOK_H
