#include "http_util.h"

#include <string>

#include <QEventLoop>
#include <QNetworkReply>
#include <QUrlQuery>

#include "no_destructor.h"

namespace MyTranslation {

Status HttpUtil::buildQuery(const Url &url, QUrlQuery &query) {
    for (const auto field : url.query) {
        query.addQueryItem(field.first.data(), field.second.data());
    }
    return Status::OK();
}

Status HttpUtil::request(const Url &url, std::string *data) {
    QNetworkRequest request;
    
    QUrl qurl(url.url.data());
    QUrlQuery urlquery;
    buildQuery(url, urlquery);
    
    QNetworkReply *reply;
    switch (url.type) {
    case GET:
        qurl.setQuery(urlquery);
        request.setUrl(qurl);
        reply = qnam.get(request);
        break;
    case POST:
        request.setUrl(qurl);
        request.setHeader(QNetworkRequest::ContentTypeHeader,
                          "application/x-www-form-urlencoded");
        reply =
            qnam.post(request, urlquery.toString(QUrl::FullyEncoded).toUtf8());
        break;
    default:
        return Status::NetworkError("HttpUtil: unsupport request type");
    }

    QEventLoop eventLoop;
    QObject::connect(reply, &QNetworkReply::finished, &eventLoop,
                     &QEventLoop::quit);
    eventLoop.exec();

    if (reply->error() != QNetworkReply::NoError) {
        qWarning() << reply->errorString();
        return Status::NetworkError(reply->errorString().toStdString());
    }

    data->append(reply->readAll());
    reply->deleteLater();
    return Status::OK();
}

Status HttpUtil::get(const std::string &baseUrl, std::string *data) {
    Url url;
    url.type = RequestType::GET;
    url.url = baseUrl;
    return request(url, data);
}

Status HttpUtil::post(const std::string &baseUrl, std::string *data) {
    Url url;
    url.type = RequestType::POST;
    url.url = baseUrl;
    return request(url, data);
}

HttpUtil *getHttpUtil() {
    static NoDestructor<HttpUtil> singleton;
    return singleton.get();
}

} // namespace MyTranslation
