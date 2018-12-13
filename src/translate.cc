#include "translate.h"
#include "googleTranslate.h"

TranslateFactory *TranslateFactory::instance = nullptr;

std::map<TranslateApiName, Translate*>*
TranslateFactory::getTranslates(int apiFlag) {
  auto translates = new std::map<TranslateApiName, Translate*>;
  if (apiFlag & GOOGLE) {
    (*translates)["google"] = new GoogleTranslate;
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

Translate::~Translate() {
  
}
