#ifndef HTTP_UTIL_H
#define HTTP_UTIL_H

#include <initializer_list>

#include <QNetworkAccessManager>

#include "status.h"

namespace MyTranslation {
// TODO: hide impl information
class HttpUtil {
  public:
    HttpUtil() = default;
    HttpUtil(const HttpUtil &) = delete;
    HttpUtil &operator=(const HttpUtil &) = delete;

    Status get(std::string *data, const std::string &baseUrl,
               const std::initializer_list<std::pair<std::string, std::string>>
                   il = {});
    Status post(std::string *data, const std::string &baseUrl,
                const std::initializer_list<std::pair<std::string, std::string>>
                    il = {});

  private:
    QNetworkAccessManager qnam;
    enum RequestType { GET, POST };
    Status
    request(RequestType type, std::string *data, const std::string &baseUrl,
            const std::initializer_list<std::pair<std::string, std::string>>
                il = {});
};
extern HttpUtil *getHttpUtil();

} // namespace MyTranslation

#endif /* HTTP_UTIL_H */
