#include "http_util.h"
#include <QEventLoop>
#include <QNetworkReply>
#include <QUrlQuery>
#include <string>

HttpUtil *HttpUtil::instance = nullptr;

HttpUtil *HttpUtil::Instance() {
  if (!instance) {
    instance = new HttpUtil;
  }
  return instance;
}

// char *mallocResponseData (QNetworkReply *reply) {
//   auto replyDate = reply->readAll();
//   size_t dataLen = replyDate.length();
//   char *data = new char[dataLen];
//   memcpy(data, replyDate.constData(), dataLen);
//   return data;
// }

const HttpResponse HttpUtil::get(const std::string &url) {
  QNetworkRequest request;
  request.setRawHeader("accept", "*/*");
  request.setRawHeader("User-Agent",
                       "Mozilla/5.0 (X11; Linux x86_64) AppleWebKit/537.36 "
                       "(KHTML, like Gecko) Chrome/67.0.3396.87 Safari/537.36");
  request.setUrl(QUrl(url.c_str()));
  QNetworkReply *reply = qnam.get(request);
  QEventLoop eventLoop;
  QObject::connect(reply, &QNetworkReply::finished, &eventLoop,
                   &QEventLoop::quit);
  eventLoop.exec();
  int code = reply->error();
  std::string errorstr = reply->errorString().toStdString();
  std::string data = reply->readAll().toStdString();
  HttpResponse response = {
    .data = data,
    .code = code,
    .errormsg = errorstr
  };
  reply->deleteLater();
  return response;
}
