/*
 * author: Niklas Hempel
 * mail: s85509@htw-dresden.de
 * group: 22/041/65
 */

#include "qstyle.h"
#include "views/mainwindow.h"
#include <QApplication>
#include <QString>

using namespace std;

int main(int argc, char *argv[]) {
  QApplication a(argc, argv);
  a.setWindowIcon(QApplication::style()->standardIcon(QStyle::SP_DesktopIcon));
  MainWindow w;

  w.show();
  return a.exec();
}
