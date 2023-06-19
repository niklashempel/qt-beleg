#ifndef CD_HPP
#define CD_HPP

#include "medium.hpp"

class Cd : public Medium {
public:
  Cd(QUuid id, QString title, QString creator, int year, QUuid ownerId)
      : Medium(id, title, creator, year, ownerId){};
  Cd(QString title, QString creator, int year, QUuid ownerId)
      : Medium(title, creator, year, ownerId){};
  QString print() const {
    return this->getId().toString() + "," + QString::number(MediumType::CD) +
           "," + this->getTitle() + "," + this->getCreator() + "," +
           QString::number(this->getYear()) + "," +
           this->getOwnerId().toString();
  };
  QString getType() const { return "CD"; };
};

#endif // CD_HPP
