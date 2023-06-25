/*
 * author: Niklas Hempel
 * mail: s85509@htw-dresden.de
 * group: 22/041/65
 */

#ifndef PERSON_HPP
#define PERSON_HPP

#include <QDataStream>
#include <QString>
#include <QStringList>
#include <QUuid>

class Person {
private:
  QUuid id;
  QString firstName;
  QString lastName;

public:
  Person(QUuid id, QString firstName, QString lastName)
      : id(id), firstName(firstName), lastName(lastName){};
  Person(QString firstName, QString lastName)
      : Person(QUuid::createUuid(), firstName, lastName){};
  Person() : firstName(""), lastName(""){};
  QString getFirstName() const { return this->firstName; }
  QString getLastName() const { return this->lastName; }
  QUuid getId() const { return this->id; };
  void setFirstName(QString firstName) { this->firstName = firstName; }
  void setLastName(QString lastName) { this->lastName = lastName; };
  QString print() const {
    return this->id.toString() + "," + this->firstName + "," + this->lastName;
  };
  QString toString() const { return this->firstName + " " + this->lastName; };
  static Person *parse(QString line);
};

#endif // PERSON_HPP
