#ifndef PERSON_HPP
#define PERSON_HPP

#include <QString>

class Person
{
private:
    QString firstName;
    QString lastName;

public:
    Person(QString firstName, QString lastName) : firstName(firstName), lastName(lastName){};
    QString getFirstName();
    QString getLastName();
    void setFirstName(QString firstName);
    void setLastName(QString lastName);
};

#endif // PERSON_HPP