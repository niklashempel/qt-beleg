#ifndef DATASTORE_HPP
#define DATASTORE_HPP

#include "list.hpp"
#include <QFileInfo>
#include <QIODevice>
#include <QSaveFile>
#include <QString>
#include <QTextStream>
#include <QUuid>
#include <iostream>


template <class T> class Datastore {
private:
  QString file;

public:
  Datastore(QString file) : file(file){};
  void save(List<T> *data) const;
  List<T> *load();
  void update(QUuid id, T *item);
  void add(T *item);
  T *find(QUuid id);
};

template <class T> inline List<T> *Datastore<T>::load() {
  QFile file(this->file);

  List<T> *data = new List<T>();

  if (file.open(QIODevice::ReadOnly | QIODevice::Text)) {
    QTextStream stream(&file);
    while (!stream.atEnd()) {
      QString line = stream.readLine();
      if (line == "") {
        continue;
      }
      data->add(T::parse(line));
    }
    file.close();
  } else {
    std::cout << "Could not open file" << std::endl;
  }
  return data;
}

template <class T> inline void Datastore<T>::update(QUuid id, T *item) {
  QFile readFile(this->file);

  List<QString> *lines = new List<QString>;

  if (readFile.open(QIODevice::ReadOnly | QIODevice::Text)) {
    QTextStream stream(&readFile);
    while (!stream.atEnd()) {
      QString *line = new QString(stream.readLine());

      if (line->startsWith(id.toString())) {
        lines->add(new QString(item->print()));
      } else {
        if (line->isEmpty()) {
          continue;
        }

        lines->add(line);
      }
    }
    readFile.close();
  } else {
    std::cout << "Could not open readFile" << std::endl;
  }

  QSaveFile *writeFile = new QSaveFile(this->file);

  if (writeFile->open(QIODevice::WriteOnly | QIODevice::Text)) {
    QTextStream stream(writeFile);

    for (auto &line : *lines) {
      stream << line << "\n";
    }
    stream.flush();
    writeFile->commit();
  } else {
    std::cout << "Could not open writeFile" << std::endl;
  }
  delete lines;
  delete writeFile;
}

template <class T> inline void Datastore<T>::add(T *item) {
  QFile file(this->file);

  if (file.open(QIODevice::Append | QIODevice::Text)) {
    QTextStream stream(&file);
    stream << item->print() << "\n";
    stream.flush();
    file.close();
  } else {
    std::cout << "Could not open file" << std::endl;
  }
}

template <class T> inline T *Datastore<T>::find(QUuid id) {
  QFile file(this->file);

  if (file.open(QIODevice::ReadOnly | QIODevice::Text)) {
    QTextStream stream(&file);
    while (!stream.atEnd()) {
      QString line = stream.readLine();
      if (line.startsWith(id.toString())) {
        return T::parse(line);
      }
    }
    file.close();
  } else {
    std::cout << "Could not open file" << std::endl;
  }
  return nullptr;
}

template <class T> void Datastore<T>::save(List<T> *data) const {
  QSaveFile *file = new QSaveFile(this->file);

  if (file->open(QIODevice::WriteOnly | QIODevice::Text)) {
    QTextStream stream(file);

    for (auto &item : *data) {
      stream << item.print() << "\n";
    }

    stream.flush();
    file->commit();
  }
  delete file;
}

#endif // DATASTORE_HPP
