#ifndef GOOGLETRANSLATE_H
#define GOOGLETRANSLATE_H

#include <qstring.h>

#include "translate.h"

namespace MyTranslation {

class GoogleTranslate : public Translate {
  public:
    GoogleTranslate();
    Status textTranslate(const std::string &, TranslateResult *result) override;
    Status imgTranslate(const std::string &, TranslateResult *result) override;
    ~GoogleTranslate() override;

  private:
    void filterTranslateText(QString &text);

    std::pair<long, long> &getTKK();
    std::pair<long, long> TKK;
    Status updateTKK();
    QString TK;
};
} // namespace Translation

#endif /* GOOGLETRANSLATE_H */
