/*
 * author: Niklas Hempel
 * mail: s85509@htw-dresden.de
 * group: 22/041/65
 */

#ifndef MEDIUM_HPP
#define MEDIUM_HPP

#include "person.hpp"
#include <QString>
#include <QStringList>
#include <QUuid>

class Medium {
private:
  QUuid id;
  QString title;
  QString creator;
  int year;
  Person *owner = NULL;
  QUuid ownerId;

public:
  Medium(QUuid id, QString title, QString creator, int year, QUuid ownerId)
      : id(id), title(title), creator(creator), year(year), ownerId(ownerId){};
  Medium(QString title, QString creator, int year, QUuid ownerId)
      : Medium(QUuid::createUuid(), title, creator, year, ownerId){};
  virtual ~Medium() = default;
  QString getTitle() const { return this->title; }
  QString getCreator() const { return this->creator; }
  int getYear() const { return this->year; }
  Person *getOwner() const { return this->owner; }
  QUuid getOwnerId() const { return this->ownerId; }
  virtual QString getType() const = 0;
  QUuid getId() const { return this->id; }
  void setTitle(QString title) { this->title = title; }
  void setCreator(QString creator) { this->creator = creator; }
  void setYear(int year) { this->year = year; }
  void setOwner(Person *owner) {
    this->owner = owner;
    this->ownerId = owner ? owner->getId() : QUuid();
  }
  virtual QString print() const = 0;
  static Medium *parse(QString line);
};

enum MediumType { BOOK, CD, DVD };

#endif // MEDIUM_HPP
