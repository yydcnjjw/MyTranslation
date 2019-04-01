#ifndef TRANSLATE_H
#define TRANSLATE_H

#include <map>
#include <string>
#include <vector>

#include "http_util.h"
#include "status.h"

namespace MyTranslation {

enum TranslateApiType {
    GOOGLE = 0x1,
    YOUDAO = 0x2,
    TENCENT = 0x4,
    BAIDU = 0x8
};
enum TranslateLanguage { AUTO, ZH, EN, JP };

struct TranslateResult {
    TranslateApiType api;
    std::string translateResult;
    std::map<std::string, std::string> extraResult;
};

// class Translate {
//   public:
//     virtual Status textTranslate(const std::string &,
//                                  TranslateResult &result) = 0;
//     virtual Status imgTranslate(const std::string &,
//                                 TranslateResult &result) = 0;

//     Translate() = default;
//     Translate(const Translate &) = delete;
//     Translate &operator=(const Translate &) = delete;

//     virtual ~Translate();
// };

class TranslateApi {
  public:
    virtual TranslateApiType getApiInfo() = 0;
    virtual Status buildUrl(Url &url, std::string &text, TranslateLanguage source, TranslateLanguage target) = 0;
    virtual Status handleResponse(std::string &response,
                                  TranslateResult &result) = 0;

    virtual ~TranslateApi() = default;
    TranslateApi() = default;
    TranslateApi(const TranslateApi &) = delete;
    TranslateApi &operator=(const TranslateApi &) = delete;
};

class Translate {
  public:
    Translate(TranslateApi *api);
    void setTranslateLanguage(TranslateLanguage source,
                                TranslateLanguage target);
    Status textTranslate(std::string &text, TranslateResult &result);
    Status imgTranslate(std::string &text, TranslateResult &result);
    
    Translate(const Translate &) = delete;
    Translate &operator=(const Translate &) = delete;

  private:
    TranslateApi *api_;
    void filterTranslateText(std::string &text);
    TranslateLanguage source_;
    TranslateLanguage target_;
};

typedef std::string TranslateApiName;
class TranslateFactory {
  public:
    static TranslateFactory *getFactory();
    std::map<TranslateApiName, Translate *> *getTranslates(int apiFlag);
    Translate *getInstace(TranslateApiType api);

  private:
    static TranslateFactory *instance;
};

} // namespace MyTranslation

#endif /* TRANSLATE_H */
