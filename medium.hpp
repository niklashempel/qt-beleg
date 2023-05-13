#ifndef MEDIUM_HPP
#define MEDIUM_HPP

#include <QString>
#include <QStringList>
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
    QString getTitle() const { return this->title; }
    QString getCreator() const { return this->creator; }
    int getYear() const { return this->year; }
    Person getOwner() { return *this->owner; }
    void setTitle(QString title) { this->title = title; }
    void setCreator(QString creator) { this->creator = creator; }
    void setYear(int year) { this->year = year; }
    void setOwner(Person owner) { this->owner = &owner; }
    virtual QString print() const = 0;
    static Medium *parse(QString line);
};

enum MediumType
{
    BOOK,
    CD,
    DVD
};

#endif // MEDIUM_HPP
