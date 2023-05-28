
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

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::MainWindow), mediumStore("media.txt")
{
    ui->setupUi(this);

    InitializeButtons();
    InitializeEditMedium();
    SetAddMediumVisible(false);
    SetMediaListVisible(true);
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
}

void MainWindow::InitializeEditMedium()
{
    QFormLayout *formLayout = this->findChild<QFormLayout *>("addMediumLayout");
    this->dialogButtonBox = new QDialogButtonBox(QDialogButtonBox::Save | QDialogButtonBox::Cancel);

    formLayout->addWidget(dialogButtonBox);
}

void MainWindow::SetAddMediumVisible(bool visible)
{
    QWidget *mediumWidget = this->findChild<QWidget *>("addMediumWidget");
    mediumWidget->setVisible(visible);
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

void MainWindow::AddMedium()
{
    SetMediaListVisible(false);
    SetAddMediumVisible(true);
    QObject::disconnect(this->dialogButtonBox, &QDialogButtonBox::accepted, nullptr, nullptr);
    QObject::disconnect(this->dialogButtonBox, &QDialogButtonBox::rejected, nullptr, nullptr);
    QObject::connect(this->dialogButtonBox, &QDialogButtonBox::accepted, [=]()
                     {
                            QComboBox *mediumType = this->findChild<QComboBox *>("mediumComboBox");
                         QString text = mediumType->currentText();
                            if (mediumType->currentText() == "Book")
                            {
                                QLineEdit *titleEdit = this->findChild<QLineEdit *>("titleLineEdit");
                                QLineEdit *creatorEdit = this->findChild<QLineEdit *>("creatorLineEdit");
                                QLineEdit *yearEdit = this->findChild<QLineEdit *>("yearLineEdit");
                                Book *book = new Book(titleEdit->text(),creatorEdit->text(), yearEdit->text().toInt());
                                mediumStore.add(book);
                                SetAddMediumVisible(false);
                                SetMediaListVisible(true);
                            } });
    QObject::connect(this->dialogButtonBox, &QDialogButtonBox::rejected, [=]()
                     {
                         SetAddMediumVisible(false);
                         SetMediaListVisible(true); });
}

void MainWindow::LoadMedia()
{
    QTableWidget *table = this->findChild<QTableWidget *>("mediaTable");
    table->clear();
    table->setColumnWidth(0, 200);
    table->setColumnWidth(1, 50);
    table->setColumnCount(4);
    table->setHorizontalHeaderItem(0, new QTableWidgetItem("Title"));
    table->setHorizontalHeaderItem(1, new QTableWidgetItem("Year"));
    table->setEditTriggers(QAbstractItemView::NoEditTriggers);
    table->setSelectionBehavior(QAbstractItemView::SelectRows);
    table->setSelectionMode(QAbstractItemView::SingleSelection);
    table->setShowGrid(false);
    table->verticalHeader()->hide();

    List<Medium> *media = mediumStore.load();

    int row = 0;

    for (auto &item : *media)
    {
        table->insertRow(row);

        QTableWidgetItem *title = new QTableWidgetItem(item.getTitle());
        table->setItem(row, 0, title);

        QTableWidgetItem *year = new QTableWidgetItem(QString::number(item.getYear()));
        table->setItem(row, 1, year);

        QPushButton *deleteButton = new QPushButton("Delete");
        table->setCellWidget(row, 2, deleteButton);

        Medium *pItem = &item;
        QObject::connect(deleteButton, &QPushButton::clicked, [=]()
                         {
                             int row = table->currentRow();
                             table->removeRow(row);
                             media->remove(pItem);
                             mediumStore.save(media); });

        QPushButton *editButton = new QPushButton("Edit");
        table->setCellWidget(row, 3, editButton);
        QObject::connect(editButton, &QPushButton::clicked, [=]() {});
        row++;
    }
}
