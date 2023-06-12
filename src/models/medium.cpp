#include "medium.hpp"
#include "book.hpp"
#include "cd.hpp"
#include "dvd.hpp"
#include <QStringList>

Medium *Medium::parse(QString line)
{
    QStringList parts = line.split(",");
    switch (parts[1].toInt())
    {
    case MediumType::BOOK:
        return new Book(QUuid::fromString(parts[0]), parts[2], parts[3], parts[4].toInt(), QUuid::fromString(parts[5]));
    case MediumType::CD:
        return new Cd(QUuid::fromString(parts[0]), parts[2], parts[3], parts[4].toInt(), QUuid::fromString(parts[5]));
    case MediumType::DVD:
        return new Dvd(QUuid::fromString(parts[0]), parts[2], parts[3], parts[4].toInt(), QUuid::fromString(parts[5]));
    default:
        return NULL;
    }
}
