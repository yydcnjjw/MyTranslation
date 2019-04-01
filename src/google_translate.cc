#include "google_translate.h"

#include <string>
#include <vector>
#include <regex>
#include <locale>
#include <codecvt>

#include <qdebug.h>
#include <qjsonarray.h>
#include <qjsondocument.h>
#include <qjsonobject.h>

#include <http_util.h>

namespace MyTranslation {

namespace {
typedef std::pair<ulong, ulong> TKK_t;
TKK_t TKK = {0, 0};

Status updateTKK() {
    std::string data;
    Status s = getHttpUtil()->get("https://translate.google.cn/", &data);
    if (!s.ok()) {
        return s;
    }

    std::regex rx("tkk:\'(\\d*).(\\d*)\'");
    std::smatch m;
    if (std::regex_search(data, m, rx)) {
        TKK.first = std::stoul(m[1].str());
        TKK.second = std::stoul(m[2].str());
        qInfo() << "updateTKK: " << TKK;
    } else {
        qWarning() << "Not match TKK";
        return Status::NetworkError("Not match TKK");
    }
    return Status::OK();
}

Status getTKK(TKK_t &tkk) {
    if (TKK.first == 0 && TKK.second == 0) {
        Status s = updateTKK();
        if (!s.ok()) {
            return s;
        }
    }
    tkk = TKK;
    return Status::OK();
}
void strToWStr(std::string &str, std::wstring &wstr) {
    std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>> converter;
    wstr = converter.from_bytes(str);
}
ulong unicode(std::wstring &s, int i) {
    return static_cast<ulong>(s[i]);
}

long wr(ulong a, std::string &b) {
    std::wstring wb;
    strToWStr(b, wb);
    for (int i = 0; i < b.length() - 2; i += 3) {
        ulong ch = unicode(wb, i + 2);
        ulong d = 0;
        d = ('a' <= ch ? ch - 87 : ch - '0');
        d = ('+' == b[i + 1] ? a >> d : a << d);
        a = ('+' == b[i] ? a + d & 4294967295 : a ^ d);
    }
    return a;
}

Status getTK(std::string &tk, std::string &text) {
    std::wstring wtext;
    strToWStr(text, wtext);
    auto textLen = wtext.length();
    std::vector<long> e;
    for (int g = 0; g < textLen; g++) {
        ulong l = unicode(wtext, g);
        if (128 > l) {
            e.push_back(l);
        } else {
            if (2048 > l) {
                e.push_back(l >> 6 | 192);
            } else {
                if (55296 == (l & 64512) && g + 1 < textLen &&
                    56320 == (unicode(wtext, g+1) & 64512)) {
                    l = 65536 + ((l & 1023) << 10) +
                        (unicode(wtext, ++g) & 1023);
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
    TKK_t tkk;
    Status s = getTKK(tkk);
    if (!s.ok()) {
        return s;
    }
    ulong b = tkk.first;
    ulong a = b;
    std::string str1 = "+-a^+6";
    std::string str2 = "+-3^+b+-f";
    for (ulong i : e) {
        a += i;
        a = wr(a, str1);
    }
    a = wr(a, str2);
    a ^= tkk.second;
    0 > a && (a = (a & 2147483647) + 2147483648);
    a %= 1000000;
    tk = std::to_string(a) + "." + std::to_string(a ^ b);
    return Status::OK();
}

} // namespace

TranslateApiType GoogleTranslateApi::getApiInfo() {
    return TranslateApiType::GOOGLE;
}
Status GoogleTranslateApi::buildUrl(Url &url, std::string &text,
                                    TranslateLanguage source,
                                    TranslateLanguage target) {
    url.type = RequestType::GET;
    url.url = "https://translate.google.cn/translate_a/single";
    std::string tk;
    Status s = getTK(tk, text);
    if (!s.ok()) {
        return s;
    }
    url.query = {{"client", "webapp"}, {"sl", "auto"}, {"tl", "zh-CN"},
                 {"hl", "zh-CN"},      {"dt", "at"},   {"dt", "bd"},
                 {"dt", "ex"},         {"dt", "ld"},   {"dt", "md"},
                 {"dt", "qca"},        {"dt", "rw"},   {"dt", "rm"},
                 {"dt", "ss"},         {"dt", "t"},    {"source", "clks"},
                 {"ssel", "0"},        {"tsel", "0"},  {"kc", "1"},
                 {"tk", tk},           {"q", text}};
    return Status::OK();
}
Status GoogleTranslateApi::handleResponse(std::string &response,
                                          TranslateResult &result) {
    const char *data = response.data();
    QJsonParseError json_error;
    auto parse = QJsonDocument::fromJson(data, &json_error);
    if (json_error.error != QJsonParseError::NoError) {
        QRegExp rx("*302*");
        if (rx.exactMatch(data)) {
            Status s = updateTKK();
            if (!s.ok()) {
                exit(EXIT_FAILURE);
            }
            qWarning() << "302 error: tkk update";
            return Status::ReRequest("302 error: tkk update");
        }
        return Status::JsonParsorError(json_error.errorString().toStdString());
    }

    auto parseList = parse.array();
    if (!parseList[0].isNull()) {
        auto popularTranslation = parseList[0].toArray();
        for (auto elem : popularTranslation) {
            auto elem_0 = elem.toArray()[0];
            if (!elem_0.isNull()) {
                result.translateResult +=
                    elem_0.toString().toStdString() + "\n";
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
    return Status::OK();
}

} // namespace MyTranslation
