#include "mainwindow.h"
#include <QApplication>
#include <QString>
#include <iostream>
#include "person.hpp"
#include "datastore.hpp"

using namespace std;

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;

    Person person("Max", "Mustermann");

    Datastore<Person> personStore("persons.txt");

    personStore.save(&person);

    w.show();
    return a.exec();
}
