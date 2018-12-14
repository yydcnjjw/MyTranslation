#include "google_translate.h"

#include <http_util.h>
#include <qdebug.h>
#include <qjsonarray.h>
#include <qjsondocument.h>
#include <qjsonobject.h>
#include <qregexp.h>
#include <string>
#include <vector>

QString getTK(QString &translateText, std::pair<long, long> TKK);
long wr(long a, QString &b);

GoogleTranslate::GoogleTranslate() : TKK(0, 0) {}

std::pair<long, long> &GoogleTranslate::getTKK() {
  if (TKK.first == 0 && TKK.second == 0) {
    updateTKK();
  }
  return TKK;
}

TranslateResult GoogleTranslate::textTranslate(std::string &&text) {
  TranslateResult result;
  result.api = GOOGLE;

  QString qtext(text.c_str());
  filterTranslateText(qtext);
  
  std::string tk = getTK(qtext, getTKK()).toStdString();
  std::string url_text = QUrl::toPercentEncoding(qtext).toStdString();
  std::string url =
    "https://translate.google.cn/translate_a/"
    "single?client=webapp&sl=auto&tl=zh-CN&hl=zh-CN&dt=at&dt=bd&dt=ex&dt=ld&dt="
    "md&dt=qca&dt=rw&dt=rm&dt=ss&dt=t&source=clks&ssel=0&tsel=0&kc=1"
    "&tk=" + tk + "&q=" + url_text;
  HttpResponse response = HttpUtil::Instance()->get(url);

  if (response.code != 0) {
    result.code = 0;
    return result;
  }
  result.code = 1;

  const char *data = response.data.data();
  QJsonParseError json_error;
  auto parse = QJsonDocument::fromJson(data, &json_error);
  if (json_error.error != QJsonParseError::NoError) {
    QRegExp rx("*302*");
    if (rx.exactMatch(data)) {
      qDebug() << "network: 302 error";
      updateTKK();
      return textTranslate(std::move(text));
    }
    qDebug() << json_error.errorString();
  }

  // qDebug() << parse;
  auto parseList = parse.array();
  if (!parseList[0].isNull()) {
    auto popularTranslation = parseList[0].toArray();
    for (auto elem : popularTranslation) {
      auto elem_0 = elem.toArray()[0];
      if (!elem_0.isNull()) {
        result.translateResult += elem_0.toString().toStdString() + "\n";
      }
    }
    result.translateResult.erase(result.translateResult.end() - 1);
  }

  if (!parseList[1].isNull()) {
    auto otherTranslationList = parseList[1].toArray();
    for (auto elem : otherTranslationList) {
      auto otherTranslation = elem.toArray();
      auto wordType = otherTranslation[0].toString();
      auto wordTranslation = otherTranslation[1].toArray();
      std::string wordTypeValue;
      for (auto value : wordTranslation) {
        wordTypeValue.append(value.toString().toStdString() + " ");
      }
      result.extraResult[wordType.toStdString()] = wordTypeValue;
    }
  }
  return result;
}

TranslateResult GoogleTranslate::imgTranslate(std::string &filePath) {}

void GoogleTranslate::filterTranslateText(QString &text) {
  if (text != nullptr) {
    text = text.replace("-\n", "").replace("\n", " ").replace("_", " ").replace(
        "*", "");
  }
}

void GoogleTranslate::updateTKK() {
  // TODO: catch the function exception
  HttpResponse response = HttpUtil::Instance()->get("https://translate.google.cn/");
  if (response.code != 0) {
    qDebug() << response.errormsg.c_str();
    // throw TranslateException(response.errormsg);
  }

  QRegExp rx("tkk:\'(\\d*).(\\d*)\'");
  QString TKKStr;
  auto data = response.data.data();
  if (rx.indexIn(data) != -1) {
    TKK.first = rx.cap(1).toLong();
    TKK.second = rx.cap(2).toLong();
  } else {
    qDebug() << "not match TKK";
    // throw TranslateException("not match TKK");
  }
  // qDebug() << "TKK: " << rx.capturedTexts();
  // qDebug() << "TKK: " << m_TKK.first << "." << m_TKK.second;
}

long wr(long a, QString &b) {
  for (int i = 0; i < b.length() - 2; i += 3) {
    ushort ch = b[i + 2].unicode();
    long d = 0;
    d = ('a' <= ch ? ch - 87 : ch - '0');
    d = ('+' == b[i + 1] ? static_cast<ulong>(a) >> d : a << d);
    a = ('+' == b[i] ? a + d & 4294967295 : a ^ d);
  }
  return a;
}

QString getTK(QString &translateText, std::pair<long, long> TKK) {
  // TODO: QString to std::string
  auto textLen = translateText.length();
  std::vector<long> e;
  for (int g = 0; g < textLen; g++) {
    long l = translateText[g].unicode();
    if (128 > l) {
      e.push_back(l);
    } else {
      if (2048 > l) {
        e.push_back(l >> 6 | 192);
      } else {
        if (55296 == (l & 64512) && g + 1 < textLen &&
            56320 == (translateText[g + 1].unicode() & 64512)) {
          l = 65536 + ((l & 1023) << 10) +
              (translateText[++g].unicode() & 1023);
          e.push_back(l >> 18 | 240);
          e.push_back((l >> 12 & 63) | 128);
        } else {
          e.push_back(l >> 12 | 224);
          e.push_back((l >> 6 & 63) | 128);
        }
      }
      e.push_back((l & 63) | 128);
    }
  }

  long b = TKK.first;
  long a = b;
  QString str1 = "+-a^+6";
  QString str2 = "+-3^+b+-f";
  for (long i : e) {
    a += i;
    a = wr(a, str1);
  }
  a = wr(a, str2);
  a ^= TKK.second;
  0 > a && (a = (a & 2147483647) + 2147483648);
  a %= 1000000;
  return QString::fromStdString(std::to_string(a) + "." +
                                std::to_string(a ^ b));
}

GoogleTranslate::~GoogleTranslate() {
  
};
