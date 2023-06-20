
#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "../lib/datastore.hpp"
#include "../models/medium.hpp"
#include "../models/person.hpp"
#include <QDialogButtonBox>
#include <QMainWindow>

QT_BEGIN_NAMESPACE
namespace Ui {
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
  void ShowAddMedium();
  void ShowAddPerson();
  void ShowEditPerson(Person *person);
  void ShowEditMedium(Medium *medium);
  void InitializeUi();
  void SetAddMediumVisible(bool);
  void SetMediaListVisible(bool);
  void SetAddPersonVisible(bool);
  void SetPersonListVisible(bool);
  Datastore<Medium> mediumStore;
  Datastore<Person> personStore;
  QDialogButtonBox *mediaDialogButtonBox;
  QDialogButtonBox *peopleDialogButtonBox;
  Medium *selectedMedium;
  Person *selectedPerson;
  void AddOrEditMedium();
  void AddOrEditPerson();
};

#endif // MAINWINDOW_H
