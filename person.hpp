#ifndef PERSON_HPP
#define PERSON_HPP

#include <QString>
#include <QDataStream>
#include <QStringList>

class Person
{
private:
    QString firstName;
    QString lastName;

public:
    Person(QString firstName, QString lastName) : firstName(firstName), lastName(lastName){};
    Person() : firstName(""), lastName(""){};
    QString getFirstName() const { return this->firstName; }
    QString getLastName() const { return this->lastName; }
    void setFirstName(QString firstName) { this->firstName = firstName; }
    void setLastName(QString lastName) { this->lastName = lastName; };
    QString print() const { return this->firstName + "," + this->lastName; };
    static Person *parse(QString line);
};

#endif // PERSON_HPP