#ifndef TRANSLATE_H
#define TRANSLATE_H

#include <map>
#include <string>
// namespace Translation {

enum TranslateApi { GOOGLE = 0x1, YOUDAO = 0x2, TENCENT = 0x4, BAIDU = 0x8 };

struct TranslateResult {
  int code;
  TranslateApi api;
  std::string translateResult;
  std::map<std::string, std::string> extraResult;
};

class Translate {
public:
  virtual TranslateResult textTranslate(std::string &&) = 0;
  virtual TranslateResult imgTranslate(std::string &) = 0;
  virtual ~Translate();
};

typedef std::string TranslateApiName;
class TranslateFactory {
public:
  static TranslateFactory *getFactory();
  std::map<TranslateApiName, Translate*>* getTranslates(int apiFlag);
  Translate* getInstace(TranslateApi api);

private:
  static TranslateFactory *instance;
};

class TranslateException : public std::exception {
public:
  TranslateException(const std::string &error) : error(error) {}
  const char *what() const noexcept {
    return std::string("Translate exception: " + error).c_str();
    // return "";
  }

private:
  std::string error;
};

// }  // Translation

#endif /* TRANSLATE_H */
