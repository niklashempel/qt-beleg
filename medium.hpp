#ifndef MEDIUM_HPP
#define MEDIUM_HPP

#include <QString>
#include "person.hpp"

class Medium
{
private:
    QString title;
    QString creator;
    int year;
    Person *owner = NULL;

public:
    Medium(QString title, QString creator, int year) : title(title), creator(creator), year(year){};
    QString getTitle();
    QString getCreator();
    int getYear();
    Person getOwner();
    void setTitle(QString title);
    void setCreator(QString creator);
    void setYear(int year);
    void setOwner(Person owner);
};

#endif // MEDIUM_HPP
