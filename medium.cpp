#include "medium.hpp"
#include "book.hpp"
#include <QStringList>

Medium *Medium::parse(QString line)
{
    QStringList parts = line.split(",");
    switch (parts[0].toInt())
    {
    case MediumType::BOOK:
        return new Book(parts[1], parts[2], parts[3].toInt());
        break;

    case MediumType::CD:
    case MediumType::DVD:
    default:
        return NULL;
    }
}
