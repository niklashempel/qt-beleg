#ifndef DATASTORE_HPP
#define DATASTORE_HPP

#include <QString>
#include <QFile>
#include <QTextStream>
#include <QIODevice>
#include <iostream>
#include <QFileInfo>

template <class T>
class Datastore
{
private:
    QString file;

public:
    Datastore(QString file) : file(file){};
    void save(T *data);
    T *load();
};

template <class T>
T *Datastore<T>::load()
{
    return nullptr;
}

template <class T>
void Datastore<T>::save(T *data)
{
    QFile file(this->file);

    if (file.open(QIODevice::ReadWrite))
    {
        QTextStream stream(&file);
        stream << data->print();
        file.close();
    }
}

#endif // DATASTORE_HPP
