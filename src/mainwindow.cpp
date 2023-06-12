
#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include <QTableWidget>
#include "datastore.hpp"
#include "models/medium.hpp"
#include "models/book.hpp"
#include "models/cd.hpp"
#include "models/dvd.hpp"
#include <QTableWidget>
#include <QStringList>
#include <QHeaderView>
#include <QPushButton>
#include <QFormLayout>
#include <QLabel>
#include <QLineEdit>
#include <QGridLayout>
#include <QDialogButtonBox>
#include <QComboBox>
#include <map>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::MainWindow),
      mediumStore(QCoreApplication::applicationDirPath() + "/media.txt"),
      personStore(QCoreApplication::applicationDirPath() + "/people.txt")
{
    ui->setupUi(this);

    InitializeUi();

    SetAddMediumVisible(false);
    SetMediaListVisible(true);
    SetAddPersonVisible(false);
    SetPersonListVisible(true);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::SetAddMediumVisible(bool visible)
{
    QWidget *mediumWidget = this->findChild<QWidget *>("addMediumWidget");
    mediumWidget->setVisible(visible);
    if (visible)
    {
        QObject::disconnect(this->mediaDialogButtonBox, &QDialogButtonBox::accepted, nullptr, nullptr);
        QObject::disconnect(this->mediaDialogButtonBox, &QDialogButtonBox::rejected, nullptr, nullptr);
        QComboBox *ownerComboBox = this->findChild<QComboBox *>("ownerComboBox");
        ownerComboBox->clear();
        List<Person> *people = personStore.load();
        ownerComboBox->addItem("None");
        ownerComboBox->setItemData(0, QVariant::fromValue(QUuid()));

        for (auto &person : *people)
        {
            ownerComboBox->addItem(person.getFirstName() + " " + person.getLastName());
            ownerComboBox->setItemData(ownerComboBox->count() - 1, QVariant::fromValue(person.getId()));
        }
    }
}

void MainWindow::SetMediaListVisible(bool visible)
{
    QWidget *mediaListWidget = this->findChild<QWidget *>("mediaListWidget");
    mediaListWidget->setVisible(visible);
    if (visible)
    {
        LoadMedia();
    }
}

void MainWindow::SetAddPersonVisible(bool visible)
{
    QWidget *personWidget = this->findChild<QWidget *>("addPersonWidget");
    personWidget->setVisible(visible);
    if (visible)
    {
        QObject::disconnect(this->peopleDialogButtonBox, &QDialogButtonBox::accepted, nullptr, nullptr);
        QObject::disconnect(this->peopleDialogButtonBox, &QDialogButtonBox::rejected, nullptr, nullptr);
    }
}

void MainWindow::SetPersonListVisible(bool visible)
{
    QWidget *peopleListWidget = this->findChild<QWidget *>("peopleListWidget");
    peopleListWidget->setVisible(visible);
    if (visible)
    {
        LoadPeople();
    }
}

void MainWindow::AddMedium()
{
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
    QObject::connect(this->mediaDialogButtonBox, &QDialogButtonBox::accepted, [=]()
                     {
                         Medium *medium;
                         QComboBox *mediumType = this->findChild<QComboBox *>("mediumComboBox");
                         QString text = mediumType->currentText();
                        QLineEdit *titleEdit = this->findChild<QLineEdit *>("titleLineEdit");
                        QLineEdit *creatorEdit = this->findChild<QLineEdit *>("creatorLineEdit");
                        QLineEdit *yearEdit = this->findChild<QLineEdit *>("yearLineEdit");
                        QComboBox *ownerComboBox = this->findChild<QComboBox *>("ownerComboBox");
                        QUuid ownerId = ownerComboBox->currentData().value<QUuid>();
                         if (text == "Book")
                         {
                             medium = new Book(titleEdit->text(), creatorEdit->text(), yearEdit->text().toInt(), ownerId);
                         }
                         else if (text == "CD")
                         {
                             medium = new Cd(titleEdit->text(), creatorEdit->text(), yearEdit->text().toInt(), ownerId);
                         }
                         else if (text == "DVD")
                         {
                             medium = new Dvd(titleEdit->text(), creatorEdit->text(), yearEdit->text().toInt(), ownerId);
                         }
                         else
                         {
                             std::cout << "Unknown medium type: " + text.toStdString() << std::endl;
                             return;
                         }
                         mediumStore.add(medium);
                         delete medium;
                             SetAddMediumVisible(false);
                             SetMediaListVisible(true); });

    QObject::connect(this->mediaDialogButtonBox, &QDialogButtonBox::rejected, [=]()
                     {
                         SetAddMediumVisible(false);
                         SetMediaListVisible(true); });
}

void MainWindow::AddPerson()
{
    SetPersonListVisible(false);
    SetAddPersonVisible(true);
    QLineEdit *firstName = this->findChild<QLineEdit *>("firstNameLineEdit");
    QLineEdit *lastName = this->findChild<QLineEdit *>("lastNameLineEdit");
    firstName->setText("");
    lastName->setText("");
    QObject::connect(this->peopleDialogButtonBox, &QDialogButtonBox::accepted, [=]()
                     {
        QLineEdit *firstName = this->findChild<QLineEdit *>("firstNameLineEdit");
        QLineEdit *lastName = this->findChild<QLineEdit *>("lastNameLineEdit");
        Person *person = new Person(firstName->text(), lastName->text());
        personStore.add(person);
        delete person;
        SetAddPersonVisible(false);
        SetPersonListVisible(true); });

    QObject::connect(this->peopleDialogButtonBox, &QDialogButtonBox::rejected, [=]()
                     {
        SetAddPersonVisible(false);
        SetPersonListVisible(true); });
}

void MainWindow::EditPerson(Person *person)
{
    SetPersonListVisible(false);
    SetAddPersonVisible(true);
    QLineEdit *firstName = this->findChild<QLineEdit *>("firstNameLineEdit");
    QLineEdit *lastName = this->findChild<QLineEdit *>("lastNameLineEdit");
    firstName->setText(person->getFirstName());
    lastName->setText(person->getLastName());
    QObject::connect(this->peopleDialogButtonBox, &QDialogButtonBox::accepted, [=]()
                     {
        person->setFirstName(firstName->text());
        person->setLastName(lastName->text());
        personStore.update(person->getId(), person);
        SetAddPersonVisible(false);
        SetPersonListVisible(true); });

    QObject::connect(this->peopleDialogButtonBox, &QDialogButtonBox::rejected, [=]()
                     {
        SetAddPersonVisible(false);
        SetPersonListVisible(true); });
}

void MainWindow::EditMedium(Medium *medium)
{
    SetMediaListVisible(false);
    SetAddMediumVisible(true);
    QLineEdit *title = this->findChild<QLineEdit *>("titleLineEdit");
    QLineEdit *creator = this->findChild<QLineEdit *>("creatorLineEdit");
    QLineEdit *year = this->findChild<QLineEdit *>("yearLineEdit");
    QComboBox *ownerComboBox = this->findChild<QComboBox *>("ownerComboBox");
    title->setText(medium->getTitle());
    creator->setText(medium->getCreator());
    year->setText(QString::number(medium->getYear()));
    ownerComboBox->setCurrentIndex(ownerComboBox->findData(QVariant::fromValue(medium->getOwnerId())));

    QObject::connect(this->mediaDialogButtonBox, &QDialogButtonBox::accepted, [=]()
                     {
        medium->setTitle(title->text());
        medium->setCreator(creator->text());
        medium->setYear(year->text().toInt());
        if (ownerComboBox->currentIndex() != -1)
        {
            QUuid id = ownerComboBox->currentData().toUuid();
            if (id.isNull())
            {
                medium->setOwner(nullptr);
            }
            else
            {
                Person *owner = personStore.find(id);
                if (owner != nullptr)
                {
                    medium->setOwner(owner);
                }
            }
            mediumStore.update(medium->getId(), medium);
            SetAddMediumVisible(false);
            SetMediaListVisible(true);
        } });

    QObject::connect(this->mediaDialogButtonBox, &QDialogButtonBox::rejected, [=]()
                     {
        SetAddMediumVisible(false);
        SetMediaListVisible(true); });
}

void MainWindow::InitializeUi()
{
    QTableWidget *mediaTable = this->findChild<QTableWidget *>("mediaTable");
    mediaTable->setColumnCount(7);
    mediaTable->setHorizontalHeaderItem(0, new QTableWidgetItem("Type"));
    mediaTable->setHorizontalHeaderItem(1, new QTableWidgetItem("Title"));
    mediaTable->setHorizontalHeaderItem(2, new QTableWidgetItem("Creator"));
    mediaTable->setHorizontalHeaderItem(3, new QTableWidgetItem("Year"));
    mediaTable->setHorizontalHeaderItem(4, new QTableWidgetItem("Owner"));
    mediaTable->setHorizontalHeaderItem(5, new QTableWidgetItem(""));
    mediaTable->setHorizontalHeaderItem(6, new QTableWidgetItem(""));
    mediaTable->verticalHeader()->hide();

    QTableWidget *peopleTable = this->findChild<QTableWidget *>("peopleTable");
    peopleTable->setRowCount(0);
    peopleTable->setHorizontalHeaderItem(0, new QTableWidgetItem("First name"));
    peopleTable->setHorizontalHeaderItem(1, new QTableWidgetItem("Last name"));
    peopleTable->setHorizontalHeaderItem(2, new QTableWidgetItem(""));
    peopleTable->setHorizontalHeaderItem(3, new QTableWidgetItem(""));
    peopleTable->verticalHeader()->hide();

    QPushButton *addMediumButton = this->findChild<QPushButton *>("addMediumButton");
    QObject::connect(addMediumButton, &QPushButton::clicked, [=]()
                     { AddMedium(); });

    QPushButton *backButton = this->findChild<QPushButton *>("backButton");
    QObject::connect(backButton, &QPushButton::clicked, [=]()
                     { SetAddMediumVisible(false);
                       SetMediaListVisible(true); });

    QLabel *creatorLabel = this->findChild<QLabel *>("creatorLabel");
    QComboBox *mediumType = this->findChild<QComboBox *>("mediumComboBox");
    QObject::connect(mediumType, &QComboBox::currentTextChanged, [=]()
                     {
                         QString text = mediumType->currentText();
                         if (text == "Book")
                         {
                             creatorLabel->setText("Author");
                         }
                         else if(text == "CD")
                         {
                             creatorLabel->setText("Artist");
                         }else if(text == "DVD")
                         {
                             creatorLabel->setText("Director");
                         } });

    QPushButton *addPersonButton = this->findChild<QPushButton *>("addPersonButton");
    QObject::connect(addPersonButton, &QPushButton::clicked, [=]()
                     { AddPerson(); });

    QPushButton *peopleBackButton = this->findChild<QPushButton *>("peopleBackButton");
    QObject::connect(peopleBackButton, &QPushButton::clicked, [=]()
                     { SetAddPersonVisible(false);
                       SetPersonListVisible(true); });
    QFormLayout *addMediumLayout = this->findChild<QFormLayout *>("addMediumLayout");
    this->mediaDialogButtonBox = new QDialogButtonBox(QDialogButtonBox::Save | QDialogButtonBox::Cancel);

    addMediumLayout->addWidget(mediaDialogButtonBox);

    QFormLayout *addPersonLayout = this->findChild<QFormLayout *>("addPersonLayout");
    this->peopleDialogButtonBox = new QDialogButtonBox(QDialogButtonBox::Save | QDialogButtonBox::Cancel);

    addPersonLayout->addWidget(peopleDialogButtonBox);
}

void MainWindow::LoadMedia()
{
    QTableWidget *table = this->findChild<QTableWidget *>("mediaTable");
    table->setRowCount(0);

    List<Medium> *media = mediumStore.load();
    List<Person> *people = personStore.load();

    std::map<QUuid, Person> peopleMap;
    for (auto &item : *people)
    {
        peopleMap[item.getId()] = item;
    }

    int row = 0;

    for (auto &medium : *media)
    {
        table->insertRow(row);

        if (!medium.getOwnerId().isNull())
        {
            Person owner = peopleMap[medium.getOwnerId()];
            medium.setOwner(&owner);
        }

        QTableWidgetItem *type = new QTableWidgetItem(medium.getType());
        table->setItem(row, 0, type);

        QTableWidgetItem *title = new QTableWidgetItem(medium.getTitle());
        table->setItem(row, 1, title);

        QTableWidgetItem *creator = new QTableWidgetItem(medium.getCreator());
        table->setItem(row, 2, creator);

        QTableWidgetItem *year = new QTableWidgetItem(QString::number(medium.getYear()));
        table->setItem(row, 3, year);

        QTableWidgetItem *owner = new QTableWidgetItem(medium.getOwner() != nullptr ? medium.getOwner()->toString() : "");
        table->setItem(row, 4, owner);

        QPushButton *deleteButton = new QPushButton("Delete");
        table->setCellWidget(row, 5, deleteButton);

        Medium *pItem = &medium;
        QObject::connect(deleteButton, &QPushButton::clicked, [=]()
                         {
                             int row = table->currentRow();
                             table->removeRow(row);
                             media->remove(pItem);
                             mediumStore.save(media); });

        QPushButton *editButton = new QPushButton("Edit");
        table->setCellWidget(row, 6, editButton);
        QObject::connect(editButton, &QPushButton::clicked, [=]()
                         { EditMedium(pItem); });
        row++;
    }
}

void MainWindow::LoadPeople()
{
    QTableWidget *table = this->findChild<QTableWidget *>("peopleTable");
    table->setRowCount(0);

    List<Person> *people = personStore.load();

    int row = 0;

    for (auto &item : *people)
    {
        table->insertRow(row);

        QTableWidgetItem *firstName = new QTableWidgetItem(item.getFirstName());
        table->setItem(row, 0, firstName);

        QTableWidgetItem *lastName = new QTableWidgetItem(item.getLastName());
        table->setItem(row, 1, lastName);

        QPushButton *deleteButton = new QPushButton("Delete");
        table->setCellWidget(row, 2, deleteButton);

        Person *pItem = &item;
        QObject::connect(deleteButton, &QPushButton::clicked, [=]()
                         {
                             int row = table->currentRow();
                             table->removeRow(row);
                             people->remove(pItem);
                             personStore.save(people); });

        QPushButton *editButton = new QPushButton("Edit");
        table->setCellWidget(row, 3, editButton);
        QObject::connect(editButton, &QPushButton::clicked, [=]()
                         { EditPerson(pItem); });
        row++;
    }
}
