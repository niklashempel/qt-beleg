
#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include <QTableWidget>
#include "datastore.hpp"
#include "medium.hpp"
#include <QTableWidget>
#include <QStringList>
#include <QHeaderView>
#include <QPushButton>
#include "book.hpp"
#include <QFormLayout>
#include <QLabel>
#include <QLineEdit>
#include <QGridLayout>
#include <QDialogButtonBox>
#include <QComboBox>
#include "cd.hpp"
#include "dvd.hpp"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::MainWindow), mediumStore("media.txt"), personStore("people.txt")
{
    ui->setupUi(this);

    InitializeButtons();
    InitializeEditMedium();
    InitializeEditPerson();
    SetAddMediumVisible(false);
    SetMediaListVisible(true);
    SetAddPersonVisible(false);
    SetPersonListVisible(true);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::InitializeButtons()
{
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
}

void MainWindow::InitializeEditMedium()
{
    QFormLayout *formLayout = this->findChild<QFormLayout *>("addMediumLayout");
    this->mediaDialogButtonBox = new QDialogButtonBox(QDialogButtonBox::Save | QDialogButtonBox::Cancel);

    formLayout->addWidget(mediaDialogButtonBox);
}

void MainWindow::InitializeEditPerson()
{
    QFormLayout *formLayout = this->findChild<QFormLayout *>("addPersonLayout");
    this->peopleDialogButtonBox = new QDialogButtonBox(QDialogButtonBox::Save | QDialogButtonBox::Cancel);

    formLayout->addWidget(peopleDialogButtonBox);
}

void MainWindow::SetAddMediumVisible(bool visible)
{
    QWidget *mediumWidget = this->findChild<QWidget *>("addMediumWidget");
    mediumWidget->setVisible(visible);
    if (visible)
    {
        QObject::disconnect(this->mediaDialogButtonBox, &QDialogButtonBox::accepted, nullptr, nullptr);
        QObject::disconnect(this->mediaDialogButtonBox, &QDialogButtonBox::rejected, nullptr, nullptr);
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
    QObject::connect(this->mediaDialogButtonBox, &QDialogButtonBox::accepted, [=]()
                     {
                            QComboBox *mediumType = this->findChild<QComboBox *>("mediumComboBox");
                         QString text = mediumType->currentText();
                            if (text == "Book")
                            {
                                QLineEdit *titleEdit = this->findChild<QLineEdit *>("titleLineEdit");
                                QLineEdit *creatorEdit = this->findChild<QLineEdit *>("creatorLineEdit");
                                QLineEdit *yearEdit = this->findChild<QLineEdit *>("yearLineEdit");
                                Book *book = new Book(titleEdit->text(),creatorEdit->text(), yearEdit->text().toInt());
                                mediumStore.add(book);
                                delete book;
                                SetAddMediumVisible(false);
                                SetMediaListVisible(true);
                            }
                            else if (text == "CD")
                            {
                                QLineEdit *titleEdit = this->findChild<QLineEdit *>("titleLineEdit");
                                QLineEdit *creatorEdit = this->findChild<QLineEdit *>("creatorLineEdit");
                                QLineEdit *yearEdit = this->findChild<QLineEdit *>("yearLineEdit");
                                Cd *cd = new Cd(titleEdit->text(),creatorEdit->text(), yearEdit->text().toInt());
                                mediumStore.add(cd);
                                delete cd;
                                SetAddMediumVisible(false);
                                SetMediaListVisible(true);
                            }
                            else if(text == "DVD")
                            {
                                QLineEdit *titleEdit = this->findChild<QLineEdit *>("titleLineEdit");
                                QLineEdit *creatorEdit = this->findChild<QLineEdit *>("creatorLineEdit");
                                QLineEdit *yearEdit = this->findChild<QLineEdit *>("yearLineEdit");
                                Dvd *dvd = new Dvd(titleEdit->text(),creatorEdit->text(), yearEdit->text().toInt());
                                mediumStore.add(dvd);
                                delete dvd;
                                SetAddMediumVisible(false);
                                SetMediaListVisible(true);
                            } });
    QObject::connect(this->mediaDialogButtonBox, &QDialogButtonBox::rejected, [=]()
                     {
                         SetAddMediumVisible(false);
                         SetMediaListVisible(true); });
}

void MainWindow::AddPerson()
{
    SetPersonListVisible(false);
    SetAddPersonVisible(true);
    QObject::connect(this->peopleDialogButtonBox, &QDialogButtonBox::accepted, [=]()
                     {
        QLineEdit *firstName = this->findChild<QLineEdit *>("firstNameLineEdit");
        QLineEdit *lastName = this->findChild<QLineEdit *>("lastNameLineEdit");
        Person *person = new Person(firstName->text(), lastName->text());
        personStore.add(person);
        delete person;
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
    title->setText(medium->getTitle());
    creator->setText(medium->getCreator());
    year->setText(QString::number(medium->getYear()));
    QObject::connect(this->mediaDialogButtonBox, &QDialogButtonBox::accepted, [=]()
                     {
        medium->setTitle(title->text());
        medium->setCreator(creator->text());
        medium->setYear(year->text().toInt());
        mediumStore.update(medium->getId(), medium);
        SetAddMediumVisible(false);
        SetMediaListVisible(true); });

    QObject::connect(this->mediaDialogButtonBox, &QDialogButtonBox::rejected, [=]()
                     {
        SetAddMediumVisible(false);
        SetMediaListVisible(true); });
}

void MainWindow::LoadMedia()
{
    QTableWidget *table = this->findChild<QTableWidget *>("mediaTable");
    table->clear();
    table->setColumnCount(5);
    table->setHorizontalHeaderItem(0, new QTableWidgetItem("Title"));
    table->setHorizontalHeaderItem(1, new QTableWidgetItem("Creator"));
    table->setHorizontalHeaderItem(2, new QTableWidgetItem("Year"));
    table->setHorizontalHeaderItem(3, new QTableWidgetItem(""));
    table->setHorizontalHeaderItem(4, new QTableWidgetItem(""));
    table->verticalHeader()->hide();

    List<Medium> *media = mediumStore.load();

    int row = 0;

    for (auto &item : *media)
    {
        table->insertRow(row);

        QTableWidgetItem *title = new QTableWidgetItem(item.getTitle());
        table->setItem(row, 0, title);

        QTableWidgetItem *creator = new QTableWidgetItem(item.getCreator());
        table->setItem(row, 1, creator);

        QTableWidgetItem *year = new QTableWidgetItem(QString::number(item.getYear()));
        table->setItem(row, 2, year);

        QPushButton *deleteButton = new QPushButton("Delete");
        table->setCellWidget(row, 3, deleteButton);

        Medium *pItem = &item;
        QObject::connect(deleteButton, &QPushButton::clicked, [=]()
                         {
                             int row = table->currentRow();
                             table->removeRow(row);
                             media->remove(pItem);
                             mediumStore.save(media); });

        QPushButton *editButton = new QPushButton("Edit");
        table->setCellWidget(row, 4, editButton);
        QObject::connect(editButton, &QPushButton::clicked, [=]()
                         { EditMedium(pItem); });
        row++;
    }
}

void MainWindow::LoadPeople()
{
    QTableWidget *table = this->findChild<QTableWidget *>("peopleTable");
    table->clear();
    table->setHorizontalHeaderItem(0, new QTableWidgetItem("First name"));
    table->setHorizontalHeaderItem(1, new QTableWidgetItem("Last name"));
    table->setHorizontalHeaderItem(2, new QTableWidgetItem(""));
    table->setHorizontalHeaderItem(3, new QTableWidgetItem(""));
    table->verticalHeader()->hide();

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
