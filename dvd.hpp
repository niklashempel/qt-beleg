#ifndef DVD_HPP
#define DVD_HPP

#include "medium.hpp"

class Dvd : public Medium
{
public:
    Dvd(QUuid id, QString title, QString creator, int year, QUuid ownerId) : Medium(id, title, creator, year, ownerId){};
    Dvd(QString title, QString creator, int year, QUuid ownerId) : Medium(title, creator, year, ownerId){};
    QString print() const
    {
        return this->getId().toString() + "," +
               QString::number(MediumType::DVD) + "," +
               this->getTitle() + "," +
               this->getCreator() + "," +
               QString::number(this->getYear()) + "," +
               QString::number(this->getYear()) + "," + this->getOwnerId().toString();
    };
    QString getType() const { return "DVD"; };
};

#endif // DVD_HPP
