#ifndef CD_HPP
#define CD_HPP

#include "medium.hpp"

class Cd : public Medium
{
public:
    Cd(QUuid id, QString title, QString creator, int year) : Medium(id, title, creator, year){};
    Cd(QString title, QString creator, int year) : Medium(title, creator, year){};
    QString print() const { return this->getId().toString() + "," + QString::number(MediumType::CD) + "," + this->getTitle() + "," + this->getCreator() + "," + QString::number(this->getYear()); }
};

#endif // CD_HPP