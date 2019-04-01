#ifndef HTTP_UTIL_H
#define HTTP_UTIL_H

#include <initializer_list>

#include <QNetworkAccessManager>

#include "status.h"

namespace MyTranslation {
// TODO: hide impl information
enum RequestType { GET, POST };
struct Url {
    std::string url;
    std::vector<std::pair<std::string, std::string>> query;
    RequestType type;
};
typedef struct Url Url;

class HttpUtil {
  public:
    HttpUtil() = default;
    Status request(const Url &url, std::string *data);
    Status get(const std::string &baseUrl, std::string *data);
    Status post(const std::string &baseUrl, std::string *data);

    HttpUtil(const HttpUtil &) = delete;
    HttpUtil &operator=(const HttpUtil &) = delete;

  private:
    QNetworkAccessManager qnam;
    Status buildQuery(const Url &url, QUrlQuery &query);
};
extern HttpUtil *getHttpUtil();

} // namespace MyTranslation

#endif /* HTTP_UTIL_H */
