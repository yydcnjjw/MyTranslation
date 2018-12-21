#include "http_util.h"

#include <string>

#include <QEventLoop>
#include <QNetworkReply>
#include <QUrlQuery>

#include "no_destructor.h"

namespace MyTranslation {
    
Status HttpUtil::request(
    RequestType type, std::string *data, const std::string &baseUrl,
    const std::initializer_list<std::pair<std::string, std::string>> query) {
    QNetworkRequest request;
    request.setHeader(QNetworkRequest::UserAgentHeader,
                      "Mozilla/5.0 (X11; Linux x86_64) AppleWebKit/537.36 "
                      "(KHTML, like Gecko) Chrome/67.0.3396.87 Safari/537.36");
    QUrl url(baseUrl.data());
    QUrlQuery urlq;
    for (const auto field : query) {
        urlq.addQueryItem(field.first.data(), field.second.data());
    }
    
    QNetworkReply *reply;
    switch (type) {
    case GET:
        url.setQuery(urlq);
        request.setUrl(url);
        reply = qnam.get(request);
        break;
    case POST:
        request.setUrl(url);
        request.setHeader(QNetworkRequest::ContentTypeHeader, "application/x-www-form-urlencoded");
        reply = qnam.post(request, urlq.toString(QUrl::FullyEncoded).toUtf8());
        break;
    default:
        return Status::NetworkError("unsupport request type");
    }
    
    QEventLoop eventLoop;
    QObject::connect(reply, &QNetworkReply::finished, &eventLoop,
                     &QEventLoop::quit);
    eventLoop.exec();
    
    if (reply->error() != QNetworkReply::NoError) {        
        return Status::NetworkError(reply->errorString().toStdString());
    }
    
    data->append(reply->readAll());
    reply->deleteLater();
    return Status::OK();
}

Status HttpUtil::get(
    std::string *data, const std::string &baseUrl,
    std::initializer_list<std::pair<std::string, std::string>> query) {
    return request(GET, data, baseUrl, query);
}

Status HttpUtil::post(
    std::string *data, const std::string &baseUrl,
    std::initializer_list<std::pair<std::string, std::string>> query) {
    return request(POST, data, baseUrl, query);
}

HttpUtil *getHttpUtil() {
    static NoDestructor<HttpUtil> singleton;
    return singleton.get();
}

} // namespace Translation
