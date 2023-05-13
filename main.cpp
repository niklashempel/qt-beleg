#include "mainwindow.h"
#include <QApplication>
#include <QString>
#include <iostream>
#include "person.hpp"
#include "datastore.hpp"
#include "list.hpp"
#include "book.hpp"
#include "medium.hpp"

using namespace std;

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;

    Datastore<Person> personStore("persons.txt");
    Datastore<Medium> mediumStore("media.txt");

    List<Person *> persons = personStore.load();
    List<Medium *> media = mediumStore.load();

    w.show();
    return a.exec();
}
