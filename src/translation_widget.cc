#include "translation_widget.h"

#include <QClipboard>
#include <QMimeData>
#include <qapplication.h>

#include "no_destructor.h"
#include "screen_shoot.h"
#include "translate.h"

namespace MyTranslation {

TranslationWidget::TranslationWidget() { initUi(); }
void TranslationWidget::initUi() {
    setFixedWidth(450);
    setWindowFlag(Qt::WindowStaysOnTopHint);
    contentLabel_ = new QLabel();
    contentLabel_->setWordWrap(true);
    contentLabel_->setTextInteractionFlags(Qt::TextSelectableByMouse);
    contentLabel_->setFixedWidth(400);
}

Status TranslationWidget::requestTranslate(const QString &text) {
    QString translation;
    static std::map<TranslateApiName, Translate *> *translates;
    if (translates == nullptr) {
        translates = TranslateFactory::getFactory()->getTranslates(GOOGLE);
    }
    for (std::pair<TranslateApiName, Translate *> translate : *translates) {
        QString apiName(translate.first.c_str());
        Translate *translateApi = translate.second;
        TranslateResult result;
        Status s = translateApi->textTranslate(text.toStdString(), &result);
        if (!s.ok()) {
            return s;
        }

        translation.append((result.translateResult + "\n").c_str());
        for (std::pair<std::string, std::string> extra : result.extraResult) {
            translation.append(
                (extra.first + "\n" + extra.second + "\n").c_str());
        }
    }
    translation.remove(translation.length() - 1, 1);
    showTranslation();
    return Status::OK();
}

void TranslationWidget::showTranslation() {
    move(QCursor::pos());
    contentLabel_->setText(content_.data());
    show();
    activateWindow();
}

void SelectTranslationWidget::Translation() {
    const QClipboard *clipboard = QApplication::clipboard();
    const QMimeData *mimeData = clipboard->mimeData(QClipboard::Selection);
    if (mimeData->hasText()) {
        QString text = clipboard->text(QClipboard::Selection);
        requestTranslate(text);
    }
}

void ClipScreenTranslationWidget::Translation() {
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

TranslationWidget *getSelectTranslationWidget() {
    static NoDestructor<SelectTranslationWidget> singleton;
    return singleton.get();
}
    
TranslationWidget *getClipScreenTranslationWidget() {
    static NoDestructor<ClipScreenTranslationWidget> singleton;
    return singleton.get();
}

} // namespace MyTranslation
