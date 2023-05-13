#include "mainwindow.h"
#include <QApplication>
#include <QString>
#include <iostream>
#include "person.hpp"
#include "datastore.hpp"
#include "list.hpp"

using namespace std;

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;

    Datastore<Person> personStore("persons.txt");

    List<Person> persons = personStore.load();

    w.show();
    return a.exec();
}
