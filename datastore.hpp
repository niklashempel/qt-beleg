#ifndef DATASTORE_HPP
#define DATASTORE_HPP

#include <iostream>
#include <QIODevice>
#include <QFileInfo>
#include <QSaveFile>
#include <QString>
#include <QTextStream>
#include "list.hpp"

template <class T>
class Datastore
{
private:
    QString file;

public:
    Datastore(QString file) : file(file){};
    void save(List<T> *data) const;
    List<T> *load();
};

template <class T>
inline List<T> *Datastore<T>::load()
{
    QFile file(this->file);

    List<T> *data = new List<T>();

    if (file.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        QTextStream stream(&file);
        while (!stream.atEnd())
        {
            QString line = stream.readLine();
            data->add(T::parse(line));
        }
    }
    else
    {
        std::cout << "Could not open file" << std::endl;
    }
    return data;
}

template <class T>
void Datastore<T>::save(List<T> *data) const
{
    QSaveFile *file = new QSaveFile(this->file);

    if (file->open(QIODevice::WriteOnly | QIODevice::Text))
    {
        QTextStream stream(file);

        Node<T> *current = data->getHead();

        while (current != nullptr && current->item != nullptr)
        {
            stream << current->item->print() << "\n";
            current = current->next;
        }

        stream.flush();
        file->commit();
    }
    delete file;
}

#endif // DATASTORE_HPP
