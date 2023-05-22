
#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include <QTableWidget>
#include "person.hpp"
#include "datastore.hpp"
#include "medium.hpp"
#include <QTableWidget>
#include <QStringList>
#include <QHeaderView>
#include <QPushButton>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    TableWidgetDisplay();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::TableWidgetDisplay()
{
    QTableWidget *table = new QTableWidget(this);

    table->move(0, 0);
    table->setColumnCount(3);
    table->setHorizontalHeaderItem(0, new QTableWidgetItem("Title"));
    table->setHorizontalHeaderItem(1, new QTableWidgetItem("Year"));
    table->setColumnWidth(0, 200);
    table->setColumnWidth(1, 50);
    table->setEditTriggers(QAbstractItemView::NoEditTriggers);
    table->setSelectionBehavior(QAbstractItemView::SelectRows);
    table->setSelectionMode(QAbstractItemView::SingleSelection);
    table->setShowGrid(false);
    table->verticalHeader()->hide();

    Datastore<Medium> mediumStore("media.txt");

    List<Medium> *media = mediumStore.load();

    int row = 0;

    Node<Medium> *current = media->getHead();

    while (current != nullptr && current->item != nullptr)
    {
        table->insertRow(row);

        QTableWidgetItem *title = new QTableWidgetItem(current->item->getTitle());
        table->setItem(row, 0, title);

        QTableWidgetItem *year = new QTableWidgetItem(QString::number(current->item->getYear()));
        table->setItem(row, 1, year);

        QPushButton *deleteButton = new QPushButton("Delete");
        table->setCellWidget(row, 2, deleteButton);

        QObject::connect(deleteButton, &QPushButton::clicked, [=]()
                         {
                             int row = table->currentRow();
                             table->removeRow(row);
                             media->remove(current->item);
                             mediumStore.save(media); });
        row++;
        current = current->next;
    }

    this->setCentralWidget(table);
}
