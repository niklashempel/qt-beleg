
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

  initializeUi();

  setAddMediumVisible(false);
  setMediaListVisible(true);
  setAddPersonVisible(false);
  setPersonListVisible(true);
}

MainWindow::~MainWindow() { delete ui; }

void MainWindow::setAddMediumVisible(bool visible) {
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

void MainWindow::setMediaListVisible(bool visible) {
  QWidget *mediaListWidget = this->findChild<QWidget *>("mediaListWidget");
  mediaListWidget->setVisible(visible);
  if (visible) {
    loadMedia();
  }
}

void MainWindow::setAddPersonVisible(bool visible) {
  QWidget *personWidget = this->findChild<QWidget *>("addPersonWidget");
  personWidget->setVisible(visible);
}

void MainWindow::setPersonListVisible(bool visible) {
  QWidget *peopleListWidget = this->findChild<QWidget *>("peopleListWidget");
  peopleListWidget->setVisible(visible);
  if (visible) {
    loadPeople();
  }
}

void MainWindow::showAddMedium() {
  setMediaListVisible(false);
  setAddMediumVisible(true);
  QLineEdit *title = this->findChild<QLineEdit *>("titleLineEdit");
  QLineEdit *creator = this->findChild<QLineEdit *>("creatorLineEdit");
  QLineEdit *year = this->findChild<QLineEdit *>("yearLineEdit");
  QComboBox *ownerComboBox = this->findChild<QComboBox *>("ownerComboBox");
  title->setText("");
  creator->setText("");
  year->setText(QDateTime::currentDateTime().toString("yyyy"));
  ownerComboBox->setCurrentIndex(0);
  this->selectedMedium = NULL;
}

void MainWindow::showAddPerson() {
  setPersonListVisible(false);
  setAddPersonVisible(true);
  QLineEdit *firstName = this->findChild<QLineEdit *>("firstNameLineEdit");
  QLineEdit *lastName = this->findChild<QLineEdit *>("lastNameLineEdit");
  firstName->setText("");
  lastName->setText("");
  this->selectedPerson = NULL;
}

void MainWindow::showEditPerson(Person *person) {
  setPersonListVisible(false);
  setAddPersonVisible(true);
  QLineEdit *firstName = this->findChild<QLineEdit *>("firstNameLineEdit");
  QLineEdit *lastName = this->findChild<QLineEdit *>("lastNameLineEdit");
  firstName->setText(person->getFirstName());
  lastName->setText(person->getLastName());
  this->selectedPerson = person;
}

void MainWindow::showEditMedium(Medium *medium) {
  setMediaListVisible(false);
  setAddMediumVisible(true);
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

void MainWindow::initializeUi() {

  QPushButton *addMediumButton =
      this->findChild<QPushButton *>("addMediumButton");
  QObject::connect(addMediumButton, &QPushButton::clicked,
                   [=]() { showAddMedium(); });

  QPushButton *backButton = this->findChild<QPushButton *>("backButton");
  QObject::connect(backButton, &QPushButton::clicked, [=]() {
    setAddMediumVisible(false);
    setMediaListVisible(true);
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
                   [=]() { showAddPerson(); });

  QPushButton *peopleBackButton =
      this->findChild<QPushButton *>("peopleBackButton");
  QObject::connect(peopleBackButton, &QPushButton::clicked, [=]() {
    setAddPersonVisible(false);
    setPersonListVisible(true);
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
  QString title = this->findChild<QLineEdit *>("titleLineEdit")->text();
  QString creator = this->findChild<QLineEdit *>("creatorLineEdit")->text();
  int year = this->findChild<QLineEdit *>("yearLineEdit")->text().toInt();
  QComboBox *ownerComboBox = this->findChild<QComboBox *>("ownerComboBox");

  if (selectedMedium == NULL) {
    Medium *medium;
    QComboBox *mediumType = this->findChild<QComboBox *>("mediumComboBox");
    QString text = mediumType->currentText();
    QUuid ownerId = ownerComboBox->currentData().value<QUuid>();
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
    selectedMedium->setTitle(title);
    selectedMedium->setCreator(creator);
    selectedMedium->setYear(year);
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
  setAddMediumVisible(false);
  setMediaListVisible(true);
}

void MainWindow::addOrEditPerson() {
  QString firstName = this->findChild<QLineEdit *>("firstNameLineEdit")->text();
  QString lastName = this->findChild<QLineEdit *>("lastNameLineEdit")->text();

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
