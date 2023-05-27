#ifndef MEDIUM_HPP
#define MEDIUM_HPP

#include <QString>
#include <QStringList>
#include "person.hpp"
#include <QUuid>

class Medium
{
private:
    QUuid id;
    QString title;
    QString creator;
    int year;
    Person *owner = NULL;

public:
    Medium(QUuid id, QString title, QString creator, int year) : id(id), title(title), creator(creator), year(year){};
    Medium(QString title, QString creator, int year) : Medium(QUuid::createUuid(), title, creator, year){};
    QString getTitle() const { return this->title; }
    QString getCreator() const { return this->creator; }
    int getYear() const { return this->year; }
    Person getOwner() { return *this->owner; }
    QUuid getId() const { return this->id; }
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
