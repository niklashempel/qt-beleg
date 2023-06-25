/*
 * author: Niklas Hempel
 * mail: s85509@htw-dresden.de
 * group: 22/041/65
 */

#include "person.hpp"
#include <QStringList>

Person *Person::parse(QString line) {
  QStringList parts = line.split(",");
  return new Person(QUuid::fromString(parts[0]), parts[1], parts[2]);
}