#include <QApplication>
#include <QMessageBox>
#include <qdebug.h>

#include "tray_widget.h"

#ifndef QT_NO_SYSTEMTRAYICON
void checkSystemTray();
void init_logger();

int main(int argc, char *argv[]) {    
    QApplication app(argc, argv);
    QApplication::setQuitOnLastWindowClosed(false);
    
    checkSystemTray();
    init_logger();
    MyTranslation::TrayWidget w;
    return app.exec();
}

void init_logger() {
    qSetMessagePattern(
        "[%{appname}][%{type}][%{time yyyyMMdd h:mm:ss.zzz t}]:\n"
        "File:%{file} Line:%{line}\nFunction:%{function}: %{message}");
}

void checkSystemTray() {
    if (!QSystemTrayIcon::isSystemTrayAvailable()) {
        QMessageBox::critical(
            nullptr, QObject::tr("Systray"),
            QObject::tr("Unable to detect system tray on this system"));
        exit(EXIT_FAILURE);
    }
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
