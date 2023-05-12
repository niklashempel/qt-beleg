#ifndef PERSON_HPP
#define PERSON_HPP

#include <QString>
#include <QDataStream>

class Person
{
private:
    QString firstName;
    QString lastName;

public:
    Person(QString firstName, QString lastName) : firstName(firstName), lastName(lastName){};
    QString getFirstName() { return this->firstName; }
    QString getLastName() { return this->lastName; }
    void setFirstName(QString firstName) { this->firstName = firstName; }
    void setLastName(QString lastName) { this->lastName = lastName; };
    QString print() { return this->firstName + "," + this->lastName; };
};

#endif // PERSON_HPP