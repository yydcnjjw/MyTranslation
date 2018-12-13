#ifndef HTTP_UTIL_H
#define HTTP_UTIL_H

#include <QNetworkAccessManager>

// namespace Translation {
  
struct HttpResponse {
  std::string data;
  int code;
  std::string errormsg;
};

class HttpUtil {
public:
  static HttpUtil *Instance();

  HttpUtil() = default;
  HttpUtil(const HttpUtil &) = delete;
  HttpUtil &operator=(const HttpUtil &) = delete;

  const HttpResponse get(const std::string &url);
  char *post(const std::string &url);

private:
  static HttpUtil *instance;
  QNetworkAccessManager qnam;
};

// } // namespace Translation

#endif /* HTTP_UTIL_H */
