#ifndef BOOK_HPP
#define BOOK_HPP

#include "medium.hpp"

class Book : public Medium {
public:
  Book(QUuid id, QString title, QString creator, int year, QUuid ownerId)
      : Medium(id, title, creator, year, ownerId){};
  Book(QString title, QString creator, int year, QUuid ownerId)
      : Medium(title, creator, year, ownerId){};
  QString print() const {
    return this->getId().toString() + "," + QString::number(MediumType::BOOK) +
           "," + this->getTitle() + "," + this->getCreator() + "," +
           QString::number(this->getYear()) + "," +
           this->getOwnerId().toString();
  };
  QString getType() const { return "Book"; };
};

#endif // BOOK_HPP
