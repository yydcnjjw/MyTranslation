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
    ~TranslationWidget() = default;
    void selectTranslation();
    void clipScreentTranslation();
    
  private:
    QLabel *contentLabel_;
    void initUi();
    void showTranslation();
    Status requestTranslate(const QString &text);
};
TranslationWidget *getTranslationWidget();
} // namespace MyTranslation

#endif /* TRANSL_INFO_WIDGET_H */
