
#include "mainwindow.h"
#include "../lib/datastore.hpp"
#include "../models/book.hpp"
#include "../models/cd.hpp"
#include "../models/dvd.hpp"
#include "../models/medium.hpp"
#include "./ui_mainwindow.h"
#include <QComboBox>
#include <QDialogButtonBox>
#include <QFormLayout>
#include <QGridLayout>
#include <QHeaderView>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include <QStringList>
#include <QTableWidget>
#include <map>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::MainWindow),
      mediumStore(QApplication::applicationDirPath() + "/media.txt"),
      personStore(QApplication::applicationDirPath() + "/people.txt") {
  ui->setupUi(this);

  InitializeUi();

  SetAddMediumVisible(false);
  SetMediaListVisible(true);
  SetAddPersonVisible(false);
  SetPersonListVisible(true);
}

MainWindow::~MainWindow() { delete ui; }

void MainWindow::SetAddMediumVisible(bool visible) {
  QWidget *mediumWidget = this->findChild<QWidget *>("addMediumWidget");
  mediumWidget->setVisible(visible);
  if (visible) {
    QComboBox *ownerComboBox = this->findChild<QComboBox *>("ownerComboBox");
    ownerComboBox->clear();
    List<Person> *people = personStore.load();
    ownerComboBox->addItem("None");
    ownerComboBox->setItemData(0, QVariant::fromValue(QUuid()));

    for (auto &person : *people) {
      ownerComboBox->addItem(person.getFirstName() + " " +
                             person.getLastName());
      ownerComboBox->setItemData(ownerComboBox->count() - 1,
                                 QVariant::fromValue(person.getId()));
    }

    ownerComboBox->model()->sort(0);
  }
}

void MainWindow::SetMediaListVisible(bool visible) {
  QWidget *mediaListWidget = this->findChild<QWidget *>("mediaListWidget");
  mediaListWidget->setVisible(visible);
  if (visible) {
    LoadMedia();
  }
}

void MainWindow::SetAddPersonVisible(bool visible) {
  QWidget *personWidget = this->findChild<QWidget *>("addPersonWidget");
  personWidget->setVisible(visible);
}

void MainWindow::SetPersonListVisible(bool visible) {
  QWidget *peopleListWidget = this->findChild<QWidget *>("peopleListWidget");
  peopleListWidget->setVisible(visible);
  if (visible) {
    LoadPeople();
  }
}

void MainWindow::ShowAddMedium() {
  SetMediaListVisible(false);
  SetAddMediumVisible(true);
  QLineEdit *title = this->findChild<QLineEdit *>("titleLineEdit");
  QLineEdit *creator = this->findChild<QLineEdit *>("creatorLineEdit");
  QLineEdit *year = this->findChild<QLineEdit *>("yearLineEdit");
  QComboBox *ownerComboBox = this->findChild<QComboBox *>("ownerComboBox");
  title->setText("");
  creator->setText("");
  year->setText("");
  ownerComboBox->setCurrentIndex(0);
  this->selectedMedium = NULL;
}

void MainWindow::ShowAddPerson() {
  SetPersonListVisible(false);
  SetAddPersonVisible(true);
  QLineEdit *firstName = this->findChild<QLineEdit *>("firstNameLineEdit");
  QLineEdit *lastName = this->findChild<QLineEdit *>("lastNameLineEdit");
  firstName->setText("");
  lastName->setText("");
  this->selectedPerson = NULL;
}

void MainWindow::ShowEditPerson(Person *person) {
  SetPersonListVisible(false);
  SetAddPersonVisible(true);
  QLineEdit *firstName = this->findChild<QLineEdit *>("firstNameLineEdit");
  QLineEdit *lastName = this->findChild<QLineEdit *>("lastNameLineEdit");
  firstName->setText(person->getFirstName());
  lastName->setText(person->getLastName());
  this->selectedPerson = person;
}

void MainWindow::ShowEditMedium(Medium *medium) {
  SetMediaListVisible(false);
  SetAddMediumVisible(true);
  QLineEdit *title = this->findChild<QLineEdit *>("titleLineEdit");
  QLineEdit *creator = this->findChild<QLineEdit *>("creatorLineEdit");
  QLineEdit *year = this->findChild<QLineEdit *>("yearLineEdit");
  QComboBox *ownerComboBox = this->findChild<QComboBox *>("ownerComboBox");
  title->setText(medium->getTitle());
  creator->setText(medium->getCreator());
  year->setText(QString::number(medium->getYear()));
  ownerComboBox->setCurrentIndex(
      ownerComboBox->findData(QVariant::fromValue(medium->getOwnerId())));
  this->selectedMedium = medium;
}

void MainWindow::InitializeUi() {

  QPushButton *addMediumButton =
      this->findChild<QPushButton *>("addMediumButton");
  QObject::connect(addMediumButton, &QPushButton::clicked,
                   [=]() { ShowAddMedium(); });

  QPushButton *backButton = this->findChild<QPushButton *>("backButton");
  QObject::connect(backButton, &QPushButton::clicked, [=]() {
    SetAddMediumVisible(false);
    SetMediaListVisible(true);
  });

  QLabel *creatorLabel = this->findChild<QLabel *>("creatorLabel");
  QComboBox *mediumType = this->findChild<QComboBox *>("mediumComboBox");
  QObject::connect(mediumType, &QComboBox::currentTextChanged, [=]() {
    QString text = mediumType->currentText();
    if (text == "Book") {
      creatorLabel->setText("Author");
    } else if (text == "CD") {
      creatorLabel->setText("Artist");
    } else if (text == "DVD") {
      creatorLabel->setText("Director");
    }
  });

  QPushButton *addPersonButton =
      this->findChild<QPushButton *>("addPersonButton");
  QObject::connect(addPersonButton, &QPushButton::clicked,
                   [=]() { ShowAddPerson(); });

  QPushButton *peopleBackButton =
      this->findChild<QPushButton *>("peopleBackButton");
  QObject::connect(peopleBackButton, &QPushButton::clicked, [=]() {
    SetAddPersonVisible(false);
    SetPersonListVisible(true);
  });
  QFormLayout *addMediumLayout =
      this->findChild<QFormLayout *>("addMediumLayout");

  QFormLayout *addPersonLayout =
      this->findChild<QFormLayout *>("addPersonLayout");

  QDialogButtonBox *mediaDialogButtonBox =
      this->findChild<QDialogButtonBox *>("mediaDialogButtonBox");
  QDialogButtonBox *peopleDialogButtonBox =
      this->findChild<QDialogButtonBox *>("peopleDialogButtonBox");

  QObject::connect(mediaDialogButtonBox, &QDialogButtonBox::rejected, [=]() {
    SetAddMediumVisible(false);
    SetMediaListVisible(true);
  });
  QObject::connect(mediaDialogButtonBox, &QDialogButtonBox::accepted,
                   [=]() { AddOrEditMedium(); });

  QObject::connect(peopleDialogButtonBox, &QDialogButtonBox::rejected, [=]() {
    SetAddPersonVisible(false);
    SetPersonListVisible(true);
  });

  QObject::connect(peopleDialogButtonBox, &QDialogButtonBox::accepted,
                   [=]() { AddOrEditPerson(); });
}

void MainWindow::LoadMedia() {
  QTableWidget *table = this->findChild<QTableWidget *>("mediaTable");
  table->setRowCount(0);

  List<Medium> *media = mediumStore.load();
  List<Person> *people = personStore.load();

  std::map<QUuid, Person> peopleMap;
  for (auto &item : *people) {
    peopleMap[item.getId()] = item;
  }

  int row = 0;

  for (auto &medium : *media) {
    table->insertRow(row);

    if (!medium.getOwnerId().isNull()) {
      Person owner = peopleMap[medium.getOwnerId()];
      medium.setOwner(&owner);
    }

    QTableWidgetItem *type = new QTableWidgetItem(medium.getType());
    table->setItem(row, 0, type);

    QTableWidgetItem *title = new QTableWidgetItem(medium.getTitle());
    table->setItem(row, 1, title);

    QTableWidgetItem *creator = new QTableWidgetItem(medium.getCreator());
    table->setItem(row, 2, creator);

    QTableWidgetItem *year =
        new QTableWidgetItem(QString::number(medium.getYear()));
    table->setItem(row, 3, year);

    QTableWidgetItem *owner = new QTableWidgetItem(
        medium.getOwner() != nullptr ? medium.getOwner()->toString() : "");
    table->setItem(row, 4, owner);

    Medium *pItem = &medium;

    QPushButton *editButton = new QPushButton("Edit");
    table->setCellWidget(row, 5, editButton);
    QObject::connect(editButton, &QPushButton::clicked,
                     [=]() { ShowEditMedium(pItem); });

    QPushButton *deleteButton = new QPushButton("Delete");
    table->setCellWidget(row, 6, deleteButton);
    QObject::connect(deleteButton, &QPushButton::clicked, [=]() {
      int row = table->currentRow();
      table->removeRow(row);
      media->remove(pItem);
      mediumStore.save(media);
    });
    row++;
  }
}

void MainWindow::LoadPeople() {
  QTableWidget *table = this->findChild<QTableWidget *>("peopleTable");
  table->setRowCount(0);

  List<Person> *people = personStore.load();

  int row = 0;

  for (auto &item : *people) {
    table->insertRow(row);

    QTableWidgetItem *firstName = new QTableWidgetItem(item.getFirstName());
    table->setItem(row, 0, firstName);

    QTableWidgetItem *lastName = new QTableWidgetItem(item.getLastName());
    table->setItem(row, 1, lastName);

    Person *pItem = &item;

    QPushButton *editButton = new QPushButton("Edit");
    table->setCellWidget(row, 2, editButton);
    QObject::connect(editButton, &QPushButton::clicked,
                     [=]() { ShowEditPerson(pItem); });

    QPushButton *deleteButton = new QPushButton("Delete");
    table->setCellWidget(row, 3, deleteButton);
    QObject::connect(deleteButton, &QPushButton::clicked, [=]() {
      int row = table->currentRow();
      table->removeRow(row);
      people->remove(pItem);
      personStore.save(people);
    });

    row++;
  }
}

void MainWindow::AddOrEditMedium() {
  QLineEdit *title = this->findChild<QLineEdit *>("titleLineEdit");
  QLineEdit *creator = this->findChild<QLineEdit *>("creatorLineEdit");
  QLineEdit *year = this->findChild<QLineEdit *>("yearLineEdit");
  QComboBox *ownerComboBox = this->findChild<QComboBox *>("ownerComboBox");

  if (selectedMedium == NULL) {
    Medium *medium;
    QComboBox *mediumType = this->findChild<QComboBox *>("mediumComboBox");
    QString text = mediumType->currentText();
    QUuid ownerId = ownerComboBox->currentData().value<QUuid>();
    if (text == "Book") {
      medium = new Book(title->text(), creator->text(), year->text().toInt(),
                        ownerId);
    } else if (text == "CD") {
      medium =
          new Cd(title->text(), creator->text(), year->text().toInt(), ownerId);
    } else if (text == "DVD") {
      medium = new Dvd(title->text(), creator->text(), year->text().toInt(),
                       ownerId);
    } else {
      std::cout << "Unknown medium type: " + text.toStdString() << std::endl;
      return;
    }
    mediumStore.add(medium);
    delete medium;
  } else {
    selectedMedium->setTitle(title->text());
    selectedMedium->setCreator(creator->text());
    selectedMedium->setYear(year->text().toInt());
    if (ownerComboBox->currentIndex() != -1) {
      QUuid id = ownerComboBox->currentData().toUuid();
      if (id.isNull()) {
        selectedMedium->setOwner(nullptr);
      } else {
        Person *owner = personStore.find(id);
        if (owner != nullptr) {
          selectedMedium->setOwner(owner);
        }
      }
      mediumStore.update(selectedMedium->getId(), selectedMedium);
    }
  }
  SetAddMediumVisible(false);
  SetMediaListVisible(true);
}

void MainWindow::AddOrEditPerson() {
  QLineEdit *firstName = this->findChild<QLineEdit *>("firstNameLineEdit");
  QLineEdit *lastName = this->findChild<QLineEdit *>("lastNameLineEdit");

  if (this->selectedPerson == NULL) {
    Person *person = new Person(firstName->text(), lastName->text());
    personStore.add(person);
    delete person;
  } else {
    this->selectedPerson->setFirstName(firstName->text());
    this->selectedPerson->setLastName(lastName->text());
    personStore.update(selectedPerson->getId(), selectedPerson);
  }
  SetAddPersonVisible(false);
  SetPersonListVisible(true);
}
