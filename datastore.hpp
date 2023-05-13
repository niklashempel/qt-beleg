#ifndef DATASTORE_HPP
#define DATASTORE_HPP

#include <iostream>
#include <QIODevice>
#include <QFileInfo>
#include <QSaveFile>
#include <QString>
#include <QTextStream>
#include "list.hpp"
#include "medium.hpp"

template <class T>
class Datastore
{
private:
    QString file;

public:
    Datastore(QString file) : file(file){};
    void save(List<T *> data) const;
    List<T *> load() const;
};

template <class T>
inline List<T *> Datastore<T>::load() const
{
    QFile file(this->file);

    int lines = 0;

    if (file.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        QTextStream stream(&file);
        while (!stream.atEnd())
        {
            stream.readLine();
            lines++;
        }
        stream.seek(0);

        List<T *> data(lines);

        for (int i = 0; i < lines; i++)
        {
            QString line = stream.readLine();
            data[i] = T::parse(line);
        }

        return data;
    }
    else
    {
        std::cout << "Could not open file" << std::endl;
        return List<T *>(0);
    }
}

template <class T>
void Datastore<T>::save(List<T *> data) const
{
    QSaveFile *file = new QSaveFile(this->file);

    if (file->open(QIODevice::WriteOnly | QIODevice::Text))
    {
        QTextStream stream(file);

        int size = data.getSize();

        for (int i = 0; i < size - 1; i++)
        {
            stream << data[i]->print() << "\n";
        }
        if (size > 0)
        {
            stream << data[size - 1]->print();
        }

        stream.flush();
        file->commit();
    }
    delete file;
}

#endif // DATASTORE_HPP
