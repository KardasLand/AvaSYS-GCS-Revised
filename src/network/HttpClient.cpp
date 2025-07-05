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

void HttpClient::disconnectClient() {
    qDebug() << "Disconnecting HttpClient";
    if (m_networkManager) {
        disconnect(m_networkManager, &QNetworkAccessManager::finished, this, nullptr);

        m_networkManager->deleteLater();
        m_networkManager = nullptr;
    }
    emit statusChanged("Disconnected");
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

    qDebug() << "Sending login request to:" << requestUrl.toString();
    emit statusChanged("LoginRequestSent");
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
        emit statusChanged("LoginReplyError");
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
        emit statusChanged("Invalid URL");
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

void HttpClient::hssRequest(const QString& url, const QByteArray &sessionId) {
    QUrl requestUrl(url);
    if (!requestUrl.isValid()) {
        emit statusChanged("Invalid URL");
        emit hssResponseReceived(QByteArray(), "Invalid URL");
        return;
    }
    QNetworkRequest request(requestUrl);
    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");
    request.setRawHeader("Cookie", sessionId);
    emit statusChanged("HSSRequestSent");
    QNetworkReply* reply = m_networkManager->get(request);
    connect(reply, &QNetworkReply::finished, this, &HttpClient::onHssReplyFinished);
}

void HttpClient::onTelemetryReplyFinished() {
    QNetworkReply* reply = qobject_cast<QNetworkReply*>(sender());
    if (!reply) return;

    QString errorString;
    QByteArray responseData;

    if (reply->error() != QNetworkReply::NoError) {
        errorString = reply->errorString();
        emit statusChanged("TelemetryReplyError");
    } else {
        responseData = reply->readAll();
        emit statusChanged("TelemetryReplySuccess");
    }

    emit telemetryResponseReceived(responseData, errorString);
    reply->deleteLater();
}

void HttpClient::onHssReplyFinished() {
    QNetworkReply* reply = qobject_cast<QNetworkReply*>(sender());
    if (!reply) return;

    QString errorString;
    QByteArray responseData;

    if (reply->error() != QNetworkReply::NoError) {
        errorString = reply->errorString();
        emit statusChanged("HSSReplyError");
    } else {
        responseData = reply->readAll();
        emit statusChanged("HSSReplySuccess");
    }

    emit hssResponseReceived(responseData, errorString);
    reply->deleteLater();
}
