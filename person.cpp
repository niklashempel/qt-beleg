#include "person.hpp"
#include <QStringList>

Person *Person::parse(QString line)
{
    QStringList parts = line.split(",");
    return new Person(parts[0], parts[1]);
}