
/*
 * author: Niklas Hempel
 * mail: s85509@htw-dresden.de
 * group: 22/041/65
 */

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
  void loadMedia();
  void loadPeople();
  void showAddMedium();
  void showAddPerson();
  void showEditPerson(Person *person);
  void showEditMedium(Medium *medium);
  void initializeUi();
  void setAddMediumVisible(bool);
  void setMediaListVisible(bool);
  void setAddPersonVisible(bool);
  void setPersonListVisible(bool);
  void addOrEditMedium();
  void addOrEditPerson();
  Datastore<Medium> mediumStore;
  Datastore<Person> personStore;
  QDialogButtonBox *mediaDialogButtonBox;
  QDialogButtonBox *peopleDialogButtonBox;
  Medium *selectedMedium;
  Person *selectedPerson;
};

#endif // MAINWINDOW_H
