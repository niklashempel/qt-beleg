#include "medium.hpp"
#include "book.hpp"
#include <QStringList>

Medium *Medium::parse(QString line)
{
    QStringList parts = line.split(",");
    switch (parts[1].toInt())
    {
    case MediumType::BOOK:
        return new Book(QUuid::fromString(parts[0]), parts[2], parts[3], parts[4].toInt());
        break;

    case MediumType::CD:
    case MediumType::DVD:
    default:
        return NULL;
    }
}
