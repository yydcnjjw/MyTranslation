#include "tray_widget.h"

#include <QApplication>
#include <QDebug>
#include <QMenu>

#include "shortcut_manager.h"

namespace MyTranslation {

TrayWidget::TrayWidget() {
    this->initOSTray();
}

void TrayWidget::initOSTray() {
    QAction *minimizeAction = new QAction(tr("Min&nimize"), this);
    QAction *maximizeAction = new QAction(tr("Ma&ximize"), this);
    QAction *restoreAction = new QAction(tr("&Restore"), this);
    QAction *quitAction = new QAction(tr("&Quit"), this);

    connect(minimizeAction, &QAction::triggered, this, &QWidget::hide);
    connect(maximizeAction, &QAction::triggered, this, &QWidget::showMaximized);
    connect(restoreAction, &QAction::triggered, this, &QWidget::showNormal);
    connect(quitAction, &QAction::triggered, qApp, &QCoreApplication::quit);

    QMenu *trayIconMenu = new QMenu(this);
    trayIconMenu->addAction(minimizeAction);
    trayIconMenu->addAction(maximizeAction);
    trayIconMenu->addAction(restoreAction);
    trayIconMenu->addSeparator();
    trayIconMenu->addAction(quitAction);

    trayIcon = new QSystemTrayIcon(this);
    trayIcon->setContextMenu(trayIconMenu);
    trayIcon->show();
}

} // namespace MyTranslation
