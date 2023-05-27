#ifndef DATASTORE_HPP
#define DATASTORE_HPP

#include <iostream>
#include <QIODevice>
#include <QFileInfo>
#include <QSaveFile>
#include <QString>
#include <QTextStream>
#include "list.hpp"
#include <QUuid>

template <class T>
class Datastore
{
private:
    QString file;

public:
    Datastore(QString file) : file(file){};
    void save(List<T> *data) const;
    List<T> *load();
    void update(QUuid id, T *item);
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
        file.close();
    }
    else
    {
        std::cout << "Could not open file" << std::endl;
    }
    return data;
}

template <class T>
inline void Datastore<T>::update(QUuid id, T *item)
{
    QFile readFile(this->file);

    List<QString> *lines = new List<QString>;

    if (readFile.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        QTextStream stream(&readFile);
        while (!stream.atEnd())
        {
            QString *line = new QString(stream.readLine());

            if (line->startsWith(id.toString()))
            {
                lines->add(new QString(item->print()));
            }
            else
            {
                lines->add(line);
            }
        }
        readFile.close();
    }
    else
    {
        std::cout << "Could not open readFile" << std::endl;
    }

    QSaveFile *writeFile = new QSaveFile(this->file);

    if (writeFile->open(QIODevice::WriteOnly | QIODevice::Text))
    {
        QTextStream stream(writeFile);

        for (auto &line : *lines)
        {
            stream << line << "\n";
        }
        stream.flush();
        writeFile->commit();
    }
    else
    {
        std::cout << "Could not open writeFile" << std::endl;
    }
    delete lines;
    delete writeFile;
}

template <class T>
void Datastore<T>::save(List<T> *data) const
{
    QSaveFile *file = new QSaveFile(this->file);

    if (file->open(QIODevice::WriteOnly | QIODevice::Text))
    {
        QTextStream stream(file);

        for (auto &item : *data)
        {
            stream << item.print() << "\n";
        }

        stream.flush();
        file->commit();
    }
    delete file;
}

#endif // DATASTORE_HPP
