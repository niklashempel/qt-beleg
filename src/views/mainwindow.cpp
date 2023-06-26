/*
 * author: Niklas Hempel
 * mail: s85509@htw-dresden.de
 * group: 22/041/65
 */

#include "mainwindow.h"
#include "../lib/datastore.hpp"
#include "../models/book.hpp"
#include "../models/cd.hpp"
#include "../models/dvd.hpp"
#include "../models/medium.hpp"
#include "./ui_mainwindow.h"
#include <QComboBox>
#include <QDateTime>
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

  initializeUi();

  setAddMediumVisible(false);
  setMediaListVisible(true);
  setAddPersonVisible(false);
  setPersonListVisible(true);
}

MainWindow::~MainWindow() { delete ui; }

void MainWindow::setAddMediumVisible(bool visible) {
  QWidget *mediumWidget = ui->addMediumWidget;
  mediumWidget->setVisible(visible);
  if (visible) {
    QComboBox *ownerComboBox = ui->ownerComboBox;
    ownerComboBox->clear();
    List<Person> *people = personStore.load();

    for (auto &person : *people) {
      ownerComboBox->addItem(person.getFirstName() + " " +
                             person.getLastName());
      ownerComboBox->setItemData(ownerComboBox->count() - 1,
                                 QVariant::fromValue(person.getId()));
    }

    ownerComboBox->model()->sort(0);
    ownerComboBox->insertItem(0, "None");
    ownerComboBox->setItemData(0, QVariant::fromValue(QUuid()));

    QPushButton *returnMediumButton = ui->returnMediumButton;

    if (selectedMedium != NULL && !selectedMedium->getOwnerId().isNull()) {
      returnMediumButton->setVisible(true);
    } else {
      returnMediumButton->setVisible(false);
    }
  }
}

void MainWindow::setMediaListVisible(bool visible) {
  QWidget *mediaListWidget = ui->mediaListWidget;
  mediaListWidget->setVisible(visible);
  if (visible) {
    loadMedia();
  }
}

void MainWindow::setAddPersonVisible(bool visible) {
  QWidget *personWidget = ui->addPersonWidget;
  personWidget->setVisible(visible);
}

void MainWindow::setPersonListVisible(bool visible) {
  QWidget *peopleListWidget = ui->peopleListWidget;
  peopleListWidget->setVisible(visible);
  if (visible) {
    loadPeople();
  }
}

void MainWindow::showAddMedium() {
  this->selectedMedium = NULL;
  setMediaListVisible(false);
  setAddMediumVisible(true);
  QLineEdit *title = ui->titleLineEdit;
  QLineEdit *creator = ui->creatorLineEdit;
  QLineEdit *year = ui->yearLineEdit;
  QComboBox *ownerComboBox = ui->ownerComboBox;
  title->setText("");
  creator->setText("");
  year->setText(QDateTime::currentDateTime().toString("yyyy"));
  ownerComboBox->setCurrentIndex(0);
}

void MainWindow::showAddPerson() {
  setPersonListVisible(false);
  setAddPersonVisible(true);
  QLineEdit *firstName = ui->firstNameLineEdit;
  QLineEdit *lastName = ui->lastNameLineEdit;
  firstName->setText("");
  lastName->setText("");
  this->selectedPerson = NULL;
}

void MainWindow::showEditPerson(Person *person) {
  setPersonListVisible(false);
  setAddPersonVisible(true);
  QLineEdit *firstName = ui->firstNameLineEdit;
  QLineEdit *lastName = ui->lastNameLineEdit;
  firstName->setText(person->getFirstName());
  lastName->setText(person->getLastName());
  this->selectedPerson = person;
}

void MainWindow::showEditMedium(Medium *medium) {
  this->selectedMedium = medium;
  setMediaListVisible(false);
  setAddMediumVisible(true);
  QLineEdit *title = ui->titleLineEdit;
  QLineEdit *creator = ui->creatorLineEdit;
  QLineEdit *year = ui->yearLineEdit;
  QComboBox *ownerComboBox = ui->ownerComboBox;
  title->setText(medium->getTitle());
  creator->setText(medium->getCreator());
  year->setText(QString::number(medium->getYear()));
  ownerComboBox->setCurrentIndex(
      ownerComboBox->findData(QVariant::fromValue(medium->getOwnerId())));
  ui->mediumComboBox->setCurrentText(medium->getType());
}

void MainWindow::initializeUi() {

  QPushButton *addMediumButton = ui->addMediumButton;
  QObject::connect(addMediumButton, &QPushButton::clicked,
                   [=]() { showAddMedium(); });

  QPushButton *backButton = ui->backButton;
  QObject::connect(backButton, &QPushButton::clicked, [=]() {
    setAddMediumVisible(false);
    setMediaListVisible(true);
  });

  QPushButton *returnMediumButton = ui->returnMediumButton;
  QObject::connect(returnMediumButton, &QPushButton::clicked, [=]() {
    if (selectedMedium != NULL) {
      QComboBox *ownerComboBox = ui->ownerComboBox;
      ownerComboBox->setCurrentText("None");
    }
  });

  QLabel *creatorLabel = ui->creatorLabel;
  QComboBox *mediumType = ui->mediumComboBox;
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

  QPushButton *addPersonButton = ui->addPersonButton;
  QObject::connect(addPersonButton, &QPushButton::clicked,
                   [=]() { showAddPerson(); });

  QPushButton *peopleBackButton = ui->peopleBackButton;
  QObject::connect(peopleBackButton, &QPushButton::clicked, [=]() {
    setAddPersonVisible(false);
    setPersonListVisible(true);
  });
  QFormLayout *addMediumLayout = ui->addMediumLayout;

  QFormLayout *addPersonLayout = ui->addPersonLayout;

  QDialogButtonBox *mediaDialogButtonBox = ui->mediaDialogButtonBox;
  QDialogButtonBox *peopleDialogButtonBox = ui->peopleDialogButtonBox;

  QObject::connect(mediaDialogButtonBox, &QDialogButtonBox::rejected, [=]() {
    setAddMediumVisible(false);
    setMediaListVisible(true);
  });
  QObject::connect(mediaDialogButtonBox, &QDialogButtonBox::accepted,
                   [=]() { addOrEditMedium(); });

  QObject::connect(peopleDialogButtonBox, &QDialogButtonBox::rejected, [=]() {
    setAddPersonVisible(false);
    setPersonListVisible(true);
  });

  QObject::connect(peopleDialogButtonBox, &QDialogButtonBox::accepted,
                   [=]() { addOrEditPerson(); });
}

void MainWindow::loadMedia() {
  QTableWidget *table = ui->mediaTable;
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
                     [=]() { showEditMedium(pItem); });

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

void MainWindow::loadPeople() {
  QTableWidget *table = ui->peopleTable;
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
                     [=]() { showEditPerson(pItem); });

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

void MainWindow::addOrEditMedium() {
  QString title = ui->titleLineEdit->text();
  QString creator = ui->creatorLineEdit->text();
  int year = ui->yearLineEdit->text().toInt();
  QComboBox *ownerComboBox = ui->ownerComboBox;
  QComboBox *mediumType = ui->mediumComboBox;
  QString text = mediumType->currentText();
  QUuid ownerId = ownerComboBox->currentData().value<QUuid>();

  if (selectedMedium == NULL) {
    Medium *medium;
    if (text == "Book") {
      medium = new Book(title, creator, year, ownerId);
    } else if (text == "CD") {
      medium = new Cd(title, creator, year, ownerId);
    } else if (text == "DVD") {
      medium = new Dvd(title, creator, year, ownerId);
    } else {
      std::cout << "Unknown medium type: " + text.toStdString() << std::endl;
      return;
    }
    mediumStore.add(medium);
    delete medium;
  } else {
    if (selectedMedium->getType() != text) {
      delete selectedMedium;
      if (text == "Book") {
        selectedMedium =
            new Book(selectedMedium->getId(), title, creator, year, ownerId);
      } else if (text == "CD") {
        selectedMedium =
            new Cd(selectedMedium->getId(), title, creator, year, ownerId);
      } else if (text == "DVD") {
        selectedMedium =
            new Dvd(selectedMedium->getId(), title, creator, year, ownerId);
      } else {
        std::cout << "Unknown medium type: " + text.toStdString() << std::endl;
        return;
      }
    } else {
      selectedMedium->setTitle(title);
      selectedMedium->setCreator(creator);
      selectedMedium->setYear(year);
    }
    if (ownerComboBox->currentIndex() != -1) {
      if (ownerId.isNull()) {
        selectedMedium->setOwner(nullptr);
      } else {
        Person *owner = personStore.find(ownerId);
        if (owner != nullptr) {
          selectedMedium->setOwner(owner);
        }
      }

      mediumStore.update(selectedMedium->getId(), selectedMedium);
    }
  }
  setAddMediumVisible(false);
  setMediaListVisible(true);
}

void MainWindow::addOrEditPerson() {
  QString firstName = ui->firstNameLineEdit->text();
  QString lastName = ui->lastNameLineEdit->text();

  if (this->selectedPerson == NULL) {
    Person *person = new Person(firstName, lastName);
    personStore.add(person);
    delete person;
  } else {
    this->selectedPerson->setFirstName(firstName);
    this->selectedPerson->setLastName(lastName);
    personStore.update(selectedPerson->getId(), selectedPerson);
  }
  setAddPersonVisible(false);
  setPersonListVisible(true);
}
