#ifndef TRANSL_INFO_WIDGET_H
#define TRANSL_INFO_WIDGET_H

#include <qlabel.h>
#include <qwidget.h>

#include "status.h"

namespace MyTranslation {
class TranslationWidget : public QWidget {
    Q_OBJECT
  public:
    explicit TranslationWidget();

    Status requestTranslate(const QString &text);

  private:
    QLabel *contentLabel_;
    std::string content_;
    void initUi();
    void showTranslation();
  public slots:
    virtual void Translation() = 0;
};

class SelectTranslationWidget : public TranslationWidget {
    Q_OBJECT
  public:
    virtual void Translation() override;
};

class ClipScreenTranslationWidget : public TranslationWidget {
    Q_OBJECT
  public:
    virtual void Translation() override;
};

extern TranslationWidget *getSelectTranslationWidget();
extern TranslationWidget *getClipScreenTranslationWidget();

} // namespace MyTranslation

#endif /* TRANSL_INFO_WIDGET_H */
