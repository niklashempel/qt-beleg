
#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "datastore.hpp"
#include "medium.hpp"
#include <QDialogButtonBox>

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
    void LoadMedia();
    void AddMedium();
    void InitializeButtons();
    void SetAddMediumVisible(bool);
    void SetMediaListVisible(bool);
    void InitializeEditMedium();
    Datastore<Medium> mediumStore;
    QDialogButtonBox *dialogButtonBox;
};

#endif // MAINWINDOW_H
