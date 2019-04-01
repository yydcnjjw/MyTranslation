#include "translation_widget.h"

#include <QClipboard>
#include <QMimeData>
#include <qapplication.h>
#include <QLayout>

#include "no_destructor.h"
#include "screen_shoot.h"
#include "translate.h"

namespace MyTranslation {

TranslationWidget::TranslationWidget() { initUi(); }
void TranslationWidget::initUi() {
    setFixedWidth(450);
    setWindowFlag(Qt::WindowStaysOnTopHint);
    contentLabel_ = new QLabel;
    contentLabel_->setWordWrap(true);
    contentLabel_->setTextInteractionFlags(Qt::TextSelectableByMouse);
    contentLabel_->setFixedWidth(400);
    QVBoxLayout *layout = new QVBoxLayout;
    layout->addWidget(contentLabel_);
    setLayout(layout);
}

Status TranslationWidget::requestTranslate(const QString &qtext) {
    QString translation;
    std::string text = qtext.toStdString();
    static std::map<TranslateApiName, Translate *> *translates;
    if (translates == nullptr) {
        translates = TranslateFactory::getFactory()->getTranslates(GOOGLE);
    }
    for (std::pair<TranslateApiName, Translate *> translate : *translates) {
        QString apiName(translate.first.c_str());
        Translate *translateApi = translate.second;
        TranslateResult result;
        Status s = translateApi->textTranslate(text, result);
        if (!s.ok()) {
            qDebug() << s.ToString().data();
            return s;
        }

        translation.append((result.translateResult + "\n").c_str());
        for (std::pair<std::string, std::string> extra : result.extraResult) {
            translation.append(
                (extra.first + "\n" + extra.second + "\n").c_str());
        }
    }
    translation.remove(translation.length() - 1, 1);
    
    contentLabel_->setText(translation);
    showTranslation();
    return Status::OK();
}

void TranslationWidget::showTranslation() {
    move(QCursor::pos());
    show();
    activateWindow();
}

void TranslationWidget::selectTranslation() {
    const QClipboard *clipboard = QApplication::clipboard();
    const QMimeData *mimeData = clipboard->mimeData(QClipboard::Selection);
    if (mimeData->hasText()) {
        QString text = clipboard->text(QClipboard::Selection);
        Status s = requestTranslate(text);
        if (!s.ok()) {
            qDebug() << s.ToString().data();
        }
    }
}

void TranslationWidget::clipScreentTranslation() {
    // ScreenShoot::Instance()->show();
    // QEventLoop eventLoop;
    // QObject::connect(ScreenShoot::Instance(), &ScreenShoot::hided,
    // &eventLoop,
    //                  &QEventLoop::quit);
    // eventLoop.exec();
    // QProcess cmd;
    // cmd.start("curl -H \"apikey:8232608b9588957\" --form "
    //           "\"file=@/home/yydcnjjw/workspace/code/project/translation/"
    //           "screen_shoot.png\"  --form \"language=jpn\" --form "
    //           "\"isOverlayRequired=true\"
    //           https://api.ocr.space/parse/image");
    // cmd.waitForStarted();
    // cmd.waitForFinished();
    // QJsonParseError json_error;
    // auto parse =
    //     QJsonDocument::fromJson(cmd.readAllStandardOutput(), &json_error);

    // if (json_error.error != QJsonParseError::NoError) {
    // }
    // QString text = "";
    // qDebug() << parse;

    // QJsonObject parseResults = parse.object();
    // auto array = parseResults.value("ParsedResults").toArray();
    // for (auto value : array) {
    //     text.append(value.toObject().value("ParsedText").toString());
    // }

    // qDebug() << text;
    // requestTranslate(text);
}

TranslationWidget *getTranslationWidget() {
    static NoDestructor<TranslationWidget> singleton;
    return singleton.get();
}

} // namespace MyTranslation
