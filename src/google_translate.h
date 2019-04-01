#ifndef GOOGLETRANSLATE_H
#define GOOGLETRANSLATE_H

#include <qstring.h>

#include "translate.h"

namespace MyTranslation {

class GoogleTranslateApi : public TranslateApi {
  public:
    GoogleTranslateApi(){};
    ~GoogleTranslateApi() override {};
    TranslateApiType getApiInfo() override;
    Status buildUrl(Url &url, std::string &text, TranslateLanguage source, TranslateLanguage target) override;
    Status handleResponse(std::string &response,
                          TranslateResult &result) override;
};
} // namespace MyTranslation

#endif /* GOOGLETRANSLATE_H */
