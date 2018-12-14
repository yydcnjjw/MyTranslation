#include "tray_widget.h"

#include <QApplication>
#include <QClipboard>
#include <QDebug>
#include <QJsonArray>
#include <QLayout>
#include <QMenu>
#include <QMessageBox>
#include <QMimeData>
#include <QMouseEvent>
#include <qjsondocument.h>
#include <qjsonobject.h>
#include <qprocess.h>

#include "screen_shoot.h"
#include "shortcut.h"
#include "translate.h"

namespace Translation {

TrayWidget::TrayWidget() {
    this->initOSTray();
    this->initShortcut();
    initTranslationDialog();
}

void TrayWidget::initShortcut() {
    Shortcut *shortcut = Shortcut::instance();
    if (shortcut->registerShortcut(ShortcutAction::CLIPBOARD, "Alt+t") ==
        false) {
        // TODO: Debug
    }
    connect(shortcut->getShortcut(ShortcutAction::CLIPBOARD),
            &QxtGlobalShortcut::activated, this,
            &TrayWidget::clipboardTranslate);

    if (shortcut->registerShortcut(ShortcutAction::SCREEN, "Ctrl+t") == false) {
        // TODO: Debug
    }
    connect(shortcut->getShortcut(ShortcutAction::SCREEN),
            &QxtGlobalShortcut::activated, this, &TrayWidget::screenTranslate);
}

void TrayWidget::initOSTray() {
    QAction *minimizeAction = new QAction(tr("Min&nimize"), this);
    QAction * maximizeAction = new QAction(tr("Ma&ximize"), this);
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

void TrayWidget::initTranslationDialog() {
    translDialog_ = new QDialog(this);
    translDialog_->setFixedWidth(450);
    translDialog_->setWindowFlag(Qt::WindowStaysOnTopHint);
    translContentLabel_ = new QLabel;
    translContentLabel_->setWordWrap(true);
    translContentLabel_->setTextInteractionFlags(Qt::TextSelectableByMouse);
    translContentLabel_->setFixedWidth(400);
    QVBoxLayout *layout = new QVBoxLayout(this);
    layout->addWidget(translContentLabel_);
    translDialog_->setLayout(layout);
}

void TrayWidget::requestTranslate(QString &text) {

    QString translation;
    static std::map<TranslateApiName, Translate *> *translates;
    if (translates == nullptr) {
        translates = TranslateFactory::getFactory()->getTranslates(GOOGLE);
    }
    for (std::pair<TranslateApiName, Translate *> translate : *translates) {
        QString apiName(translate.first.c_str());
        Translate *translateApi = translate.second;
        TranslateResult result =
            translateApi->textTranslate(std::move(text.toStdString()));
        // TODO: catch exception
        if (result.code == 1) {
            translation.append((result.translateResult + "\n").c_str());
            for (std::pair<std::string, std::string> extra :
                 result.extraResult) {
                translation.append(
                    (extra.first + "\n" + extra.second + "\n").c_str());
            }
        }
    }
    translation.remove(translation.length() - 1, 1);

    translDialog_->move(QCursor::pos());
    translContentLabel_->setText(translation);
    translDialog_->show();
    translDialog_->activateWindow();
}

void TrayWidget::clipboardTranslate() {
    const QClipboard *clipboard = QApplication::clipboard();
    const QMimeData *mimeData = clipboard->mimeData(QClipboard::Selection);
    if (mimeData->hasText()) {
        QString text = clipboard->text(QClipboard::Selection);
        requestTranslate(text);
    }
}

void TrayWidget::screenTranslate() {
    ScreenShoot::Instance()->show();
    QEventLoop eventLoop;
    QObject::connect(ScreenShoot::Instance(), &ScreenShoot::hided, &eventLoop,
                     &QEventLoop::quit);
    eventLoop.exec();
    QProcess cmd;
    cmd.start("curl -H \"apikey:8232608b9588957\" --form "
              "\"file=@/home/yydcnjjw/workspace/code/project/translation/"
              "screen_shoot.png\"  --form \"language=jpn\" --form "
              "\"isOverlayRequired=true\" https://api.ocr.space/parse/image");
    cmd.waitForStarted();
    cmd.waitForFinished();
    QJsonParseError json_error;
    auto parse =
        QJsonDocument::fromJson(cmd.readAllStandardOutput(), &json_error);

    if (json_error.error != QJsonParseError::NoError) {
        throw TranslateException("json parse fail");
    }
    QString text = "";
    qDebug() << parse;

    QJsonObject parseResults = parse.object();
    auto array = parseResults.value("ParsedResults").toArray();
    for (auto value : array) {
        text.append(value.toObject().value("ParsedText").toString());
    }

    qDebug() << text;
    requestTranslate(text);
}
} // namespace Translation
