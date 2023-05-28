#ifndef DVD_HPP
#define DVD_HPP

#include "medium.hpp"

class Dvd : public Medium
{
public:
    Dvd(QUuid id, QString title, QString creator, int year) : Medium(id, title, creator, year){};
    Dvd(QString title, QString creator, int year) : Medium(title, creator, year){};
    QString print() const { return this->getId().toString() + "," + QString::number(MediumType::DVD) + "," + this->getTitle() + "," + this->getCreator() + "," + QString::number(this->getYear()); }
};

#endif // DVD_HPP