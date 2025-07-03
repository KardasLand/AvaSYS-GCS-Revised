#include "HttpClient.h"
#include <QNetworkCookieJar>
#include <QNetworkCookie>
#include <QJsonDocument>
#include <QDebug>

HttpClient::HttpClient(QObject* parent) : QObject(parent) {
    m_networkManager = new QNetworkAccessManager(this);
    m_networkManager->setCookieJar(new QNetworkCookieJar(this));
}

HttpClient::~HttpClient() {
    // The QObject parent system will handle deletion of m_networkManager
}

void HttpClient::sendLoginRequest(const QString& url, const QString& username, const QString& password) {
    QUrl requestUrl(url);
    if (!requestUrl.isValid()) {
        emit loginFinished(-1, QByteArray(), "Invalid URL");
        return;
    }

    QJsonObject jsonData;
    jsonData["kadi"] = username;
    jsonData["sifre"] = password;
    QJsonDocument jsonDoc(jsonData);

    qDebug() << jsonDoc.toJson();

    QNetworkRequest request(requestUrl);
    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");

    QNetworkReply* reply = m_networkManager->post(request, jsonDoc.toJson());
    connect(reply, &QNetworkReply::finished, this, &HttpClient::onLoginReplyFinished);
}

void HttpClient::onLoginReplyFinished() {
    QNetworkReply* reply = qobject_cast<QNetworkReply*>(sender());
    if (!reply) return;

    QString errorString;
    int teamId = -1;
    QByteArray sessionId;

    if (reply->error() != QNetworkReply::NoError) {
        errorString = reply->errorString();
    } else {
        // Find the session_id cookie
        QList<QNetworkCookie> cookies = m_networkManager->cookieJar()->cookiesForUrl(reply->url());
        // qDebug() << "Cookies for URL:" << reply->url() << cookies;
        for (const auto& cookie : cookies) {
            qDebug() << "Cookie Name:" << cookie.name() << "Value:" << cookie.value();
            if (cookie.name() == "JSESSIONID") {
                sessionId = cookie.value();
                break;
            }
        }

        qDebug() << "Session ID:" << sessionId;
        // qDebug() << "Total Response:" << reply->readAll();
        // qDebug() << "Team id from response:" << reply->readAll().toInt();
        // response is int.
        teamId = reply->readAll().toInt();
        qDebug() << "Team ID:" << teamId;

        if (teamId == -1 || sessionId.isEmpty()) {
            errorString = "Login successful, but failed to retrieve team ID or session cookie.";
        }
    }

    emit loginFinished(teamId, sessionId, errorString);
    reply->deleteLater();
}

void HttpClient::sendTelemetryRequest(const QString& url, const QByteArray& sessionId, const QJsonObject& telemetryData) {
    QUrl requestUrl(url);
    if (!requestUrl.isValid()) {
        emit telemetryResponseReceived(QByteArray(), "Invalid URL");
        return;
    }

    QNetworkRequest request(requestUrl);
    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");
    request.setRawHeader("Cookie", sessionId);

    QJsonDocument jsonDoc(telemetryData);
    QNetworkReply* reply = m_networkManager->post(request, jsonDoc.toJson());
    connect(reply, &QNetworkReply::finished, this, &HttpClient::onTelemetryReplyFinished);
}

void HttpClient::onTelemetryReplyFinished() {
    QNetworkReply* reply = qobject_cast<QNetworkReply*>(sender());
    if (!reply) return;

    QString errorString;
    QByteArray responseData;

    if (reply->error() != QNetworkReply::NoError) {
        errorString = reply->errorString();
    } else {
        responseData = reply->readAll();
    }

    emit telemetryResponseReceived(responseData, errorString);
    reply->deleteLater();
}