#include "translate.h"

#include "google_translate.h"

namespace MyTranslation {
Translate::Translate(TranslateApi *api)
    : api_(api), source_(TranslateLanguage::AUTO),
      target_(TranslateLanguage::ZH) {}

void Translate::setTranslateLanguage(TranslateLanguage source,
                                           TranslateLanguage target) {
    source_ = source;
    target_ = target;
}

void Translate::filterTranslateText(std::string &text) {
    QString s(text.data());
    if (!s.isEmpty()) {
        s.replace("-\n", "").replace("\n", " ").replace("_", " ").replace("*",
                                                                          "");
    }
    text.clear();
    text.assign(s.toStdString());
}

Status Translate::textTranslate(std::string &text,
                                    TranslateResult &result) {
    result.api = api_->getApiInfo();
    filterTranslateText(text);
    Url url;
    Status s = api_->buildUrl(url, text, source_, target_);
    if (!s.ok()) {
        if (s.IsRerequest()) {
            qWarning() << s.ToString().data();
            return textTranslate(text, result);
        }
        return s;
    }
    std::string data;
    s = getHttpUtil()->request(url, &data);
    if (!s.ok()) {
        return s;
    }

    s = api_->handleResponse(data, result);
    if (!s.ok()) {
        return s;
    }
    return Status::OK();
}
Status Translate::imgTranslate(std::string &text, TranslateResult &result) {
    return Status::OK();
}

TranslateFactory *TranslateFactory::instance = nullptr;

std::map<TranslateApiName, Translate *> *
TranslateFactory::getTranslates(int apiFlag) {
    auto translates = new std::map<TranslateApiName, Translate *>;
    if (apiFlag & GOOGLE) {
        (*translates)["google"] = new Translate(new GoogleTranslateApi);
    } else if (apiFlag & YOUDAO) {
        // translate["youdao"] = Y
    }
    return translates;
}

TranslateFactory *TranslateFactory::getFactory() {
    if (!instance) {
        instance = new TranslateFactory;
    }
    return instance;
}

// Translate::~Translate() {}

} // namespace MyTranslation
