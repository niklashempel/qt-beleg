
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

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::MainWindow), mediumStore("media.txt")
{
    ui->setupUi(this);
    QGridLayout *gridLayout = this->findChild<QGridLayout *>("gridLayout");
    TableWidgetDisplay(gridLayout);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::EditWidgetDisplay(Medium *medium, QGridLayout *gridLayout)
{
    // Check type of medium
    if (dynamic_cast<Book *>(medium))
    {
        Book *book = dynamic_cast<Book *>(medium);
        QFormLayout *formLayout = new QFormLayout();
        QPushButton *backButton = new QPushButton("◀ Back");
        formLayout->addRow(backButton, (QWidget *)nullptr);
        formLayout->addRow("Medium", new QLabel("Book"));
        QLineEdit *titleEdit = new QLineEdit(book->getTitle());
        formLayout->addRow("Title", titleEdit);
        QLineEdit *yearEdit = new QLineEdit(QString::number(book->getYear()));
        formLayout->addRow("Year", yearEdit);

        QDialogButtonBox *dialogButtonBox = new QDialogButtonBox(QDialogButtonBox::Save | QDialogButtonBox::Cancel);

        QObject::connect(dialogButtonBox, &QDialogButtonBox::accepted, [=]()
                         {
                            book->setTitle(titleEdit->text());
                            book->setYear(yearEdit->text().toInt());
                            mediumStore.update(book->getId(), book);
                            gridLayout->removeItem(formLayout);
                            TableWidgetDisplay(gridLayout); });
        QObject::connect(dialogButtonBox, &QDialogButtonBox::rejected, [=]()
                         {  gridLayout->removeItem(formLayout);
                            TableWidgetDisplay(gridLayout); });

        formLayout->addWidget(dialogButtonBox);

        QObject::connect(backButton, &QPushButton::clicked, [=]()
                         {  gridLayout->removeItem(formLayout);
                            TableWidgetDisplay(gridLayout); });

        gridLayout->addLayout(formLayout, 1, 0);
    }
    else
    {
        throw std::invalid_argument("Unknown medium type");
    }
}

void MainWindow::TableWidgetDisplay(QGridLayout *gridLayout)
{
    QTableWidget *table = new QTableWidget(this);

    table->move(0, 0);
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

    Book book1("The Lord of the Rings", "J. R. R. Tolkien", 1954);
    Book book2("The Hobbit", "J. R. R. Tolkien", 1937);

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
        QObject::connect(editButton, &QPushButton::clicked, [=]()
                         { 
                            table->setVisible(false);
                            EditWidgetDisplay(pItem, gridLayout);
                            mediumStore.save(media); });
        row++;
    }
    gridLayout->addWidget(table);
}
