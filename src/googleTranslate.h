#ifndef GOOGLETRANSLATE_H
#define GOOGLETRANSLATE_H

#include "translate.h"
#include <qstring.h>

class GoogleTranslate : public Translate {
public:
  GoogleTranslate();
  TranslateResult textTranslate(std::string &&) override;
  TranslateResult imgTranslate(std::string &) override;
  ~GoogleTranslate() override;

private:
  void filterTranslateText(QString &text);

  std::pair<long, long> &getTKK();
  std::pair<long, long> TKK;
  void updateTKK();
  QString TK;
};

#endif /* GOOGLETRANSLATE_H */
