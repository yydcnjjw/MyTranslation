#include "trayWidget.h"

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
#include <qxtglobalshortcut.h>

#include "translate.h"
#include "screen_shoot.h"

namespace Translation {
    
TrayWidget::TrayWidget() {
  QxtGlobalShortcut *clipboard = new QxtGlobalShortcut(this);
  clipboard->setShortcut(QKeySequence("Alt+t"));
  connect(clipboard, &QxtGlobalShortcut::activated, this,
          &TrayWidget::clipboardTranslate);

  QxtGlobalShortcut *screenShoot = new QxtGlobalShortcut(this);
  screenShoot->setShortcut(QKeySequence("Ctrl+t"));
  connect(screenShoot, &QxtGlobalShortcut::activated, this,
          &TrayWidget::screenTranslate);

  fillLabel = new QLabel(tr("translation"), this);
  createActions();
  createTrayIcon();
  trayIcon->show();

  initTranslationDialog();
}

void TrayWidget::initTranslationDialog() {
  translationDialog = new QDialog(this);
  translationDialog->setFixedWidth(450);
  translationDialog->setWindowFlag(Qt::WindowStaysOnTopHint);
  translationLabel = new QLabel;
  translationLabel->setWordWrap(true);
  translationLabel->setTextInteractionFlags(Qt::TextSelectableByMouse);
  translationLabel->setFixedWidth(400);
  QVBoxLayout *layout = new QVBoxLayout(this);
  layout->addWidget(translationLabel);
  translationDialog->setLayout(layout);
}

void TrayWidget::createActions() {
  minimizeAction = new QAction(tr("Mi&nimize"), this);
  connect(minimizeAction, &QAction::triggered, this, &QWidget::hide);

  maximizeAction = new QAction(tr("Ma&ximize"), this);
  connect(maximizeAction, &QAction::triggered, this, &QWidget::showMaximized);

  restoreAction = new QAction(tr("&Restore"), this);
  connect(restoreAction, &QAction::triggered, this, &QWidget::showNormal);

  quitAction = new QAction(tr("&Quit"), this);
  connect(quitAction, &QAction::triggered, qApp, &QCoreApplication::quit);
}

void TrayWidget::createTrayIcon() {
  trayIconMenu = new QMenu(this);
  trayIconMenu->addAction(minimizeAction);
  trayIconMenu->addAction(maximizeAction);
  trayIconMenu->addAction(restoreAction);
  trayIconMenu->addSeparator();
  trayIconMenu->addAction(quitAction);

  trayIcon = new QSystemTrayIcon(this);
  trayIcon->setContextMenu(trayIconMenu);
}

void TrayWidget::requestTranslate(QString &text) {

  QString translation;
  static std::map<TranslateApiName, Translate*>* translates;
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
      for (std::pair<std::string, std::string> extra : result.extraResult) {
        translation.append((extra.first + "\n" + extra.second + "\n").c_str());
      }
    }
  }
  translation.remove(translation.length() - 1, 1);

  translationDialog->move(QCursor::pos());
  translationLabel->setText(translation);
  translationDialog->show();
  translationDialog->activateWindow();
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
}  // Translation
