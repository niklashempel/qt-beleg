#ifndef BOOK_HPP
#define BOOK_HPP

#include "medium.hpp"

class Book : public Medium
{
public:
    Book(QUuid id, QString title, QString creator, int year) : Medium(id, title, creator, year){};
    Book(QString title, QString creator, int year) : Medium(title, creator, year){};
    QString print() const { return this->getId().toString() + "," + QString::number(MediumType::BOOK) + "," + this->getTitle() + "," + this->getCreator() + "," + QString::number(this->getYear()); }
};

#endif // BOOK_HPP