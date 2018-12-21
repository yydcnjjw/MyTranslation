#ifndef TRANSLATE_H
#define TRANSLATE_H

#include <map>
#include <string>

#include "status.h"

namespace MyTranslation {

enum TranslateApi { GOOGLE = 0x1, YOUDAO = 0x2, TENCENT = 0x4, BAIDU = 0x8 };

struct TranslateResult {
    TranslateApi api;
    std::string translateResult;
    std::map<std::string, std::string> extraResult;
};

class Translate {
  public:
    virtual Status textTranslate(const std::string &, TranslateResult *result) = 0;
    virtual Status imgTranslate(const std::string &, TranslateResult *result) = 0;

    Translate() = default;
    Translate(const Translate &) = delete;
    Translate &operator=(const Translate &) = delete;

    virtual ~Translate();
};

typedef std::string TranslateApiName;
class TranslateFactory {
  public:
    static TranslateFactory *getFactory();
    std::map<TranslateApiName, Translate *> *getTranslates(int apiFlag);
    Translate *getInstace(TranslateApi api);

  private:
    static TranslateFactory *instance;
};
    
} // namespace Translation

#endif /* TRANSLATE_H */
