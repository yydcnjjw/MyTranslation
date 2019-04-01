#include "tray_widget.h"

#include <QApplication>
#include <QDebug>
#include <QMenu>

#include "shortcut_manager.h"

namespace MyTranslation {

TrayWidget::TrayWidget() {
    this->initOSTray();
    getShortcutManager();
}

void TrayWidget::initOSTray() {
    QAction *restoreAction = new QAction(tr("&Restore"), this);
    QAction *quitAction = new QAction(tr("&Quit"), this);
    connect(restoreAction, &QAction::triggered, this, &QWidget::showNormal);
    connect(quitAction, &QAction::triggered, qApp, &QCoreApplication::quit);

    QMenu *trayIconMenu = new QMenu(this);
    trayIconMenu->addAction(restoreAction);
    trayIconMenu->addSeparator();
    trayIconMenu->addAction(quitAction);

    trayIcon = new QSystemTrayIcon(this);
    trayIcon->setContextMenu(trayIconMenu);
    trayIcon->show();
}

} // namespace MyTranslation
