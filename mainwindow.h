
#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "datastore.hpp"
#include "medium.hpp"
#include "person.hpp"
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
    void LoadPeople();
    void AddMedium();
    void AddPerson();
    void EditPerson(Person *person);
    void EditMedium(Medium *medium);
    void InitializeButtons();
    void SetAddMediumVisible(bool);
    void SetMediaListVisible(bool);
    void SetAddPersonVisible(bool);
    void SetPersonListVisible(bool);
    void InitializeEditMedium();
    void InitializeEditPerson();
    Datastore<Medium> mediumStore;
    Datastore<Person> personStore;
    QDialogButtonBox *mediaDialogButtonBox;
    QDialogButtonBox *peopleDialogButtonBox;
};

#endif // MAINWINDOW_H
