#include <QApplication>
#include <QMessageBox>

#include "tray_widget.h"

#ifndef QT_NO_SYSTEMTRAYICON

int main(int argc, char *argv[]) {
  QApplication app(argc, argv);
  if (!QSystemTrayIcon::isSystemTrayAvailable()) {
    QMessageBox::critical(
        nullptr, QObject::tr("Systray"),
        QObject::tr("Unable to detect system tray on this system"));
    return 1;
  }
  QApplication::setQuitOnLastWindowClosed(false);  

  Translation::TrayWidget w;
  return app.exec();
}

#else

#include <QDebug>
#include <QLabel>

int main(int argc, char *argv[]) {
  TApplication app(argc, argv);

  QString text("QSystemTrayIcon is not supported on this platform");
  QLabel *label = new QLabel(text);
  label->setWordWrap(true);

  label->show();
  qDebug() << text;

  app.exec();
}

#endif
