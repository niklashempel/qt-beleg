
#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "datastore.hpp"
#include "medium.hpp"
#include "qgridlayout.h"

QT_BEGIN_NAMESPACE
namespace Ui
{
    class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow

{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private:
    Ui::MainWindow *ui;
    void TableWidgetDisplay(QGridLayout *);
    void EditWidgetDisplay(Medium *medium, QGridLayout *);
    Datastore<Medium> mediumStore;
};

#endif // MAINWINDOW_H
