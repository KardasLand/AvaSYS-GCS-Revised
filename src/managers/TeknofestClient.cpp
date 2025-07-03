//
// Created by kardasland on 6/27/25.
//

#include "TeknofestClient.h"

#include <utility>

#include "src/core/AppContext.h"

TeknofestClient::TeknofestClient(QObject *parent) : QObject{parent} {
    qRegisterMetaType<QJsonArray>("QJsonArray");
    m_httpClient = new HttpClient(this);

    connect(m_httpClient, &HttpClient::loginFinished, this, &TeknofestClient::onLoginFinished);
    connect(m_httpClient, &HttpClient::telemetryResponseReceived, this, &TeknofestClient::onTelemetryResponse);
}

void TeknofestClient::login() const {
    m_httpClient->sendLoginRequest(
            m_serverProperties.url + "/giris",
            m_serverProperties.teknofestAuthProperty.username,
            m_serverProperties.teknofestAuthProperty.password
        );
}

void TeknofestClient::startFetchingTelemetry() {
}

void TeknofestClient::stopFetchingTelemetry() {
}

void TeknofestClient::transmitTelemetryRequest(QJsonObject &vehicleData) {
    m_httpClient->sendTelemetryRequest(
        m_serverProperties.url + "/telemetri_gonder",
        m_sessionId,
        vehicleData
    );
}

void TeknofestClient::fetchTelemetry() {

}

void TeknofestClient::onLoginFinished(int teamId, const QByteArray &sessionId, const QString &errorString) {
    if (!errorString.isEmpty()) {
        qWarning() << "Teknofest login failed:" << errorString;
        emit loginFailed(errorString);
        return;
    }
    m_serverProperties.set_takimid(QString::number(teamId));
    m_serverProperties.session_id = sessionId;
    qDebug() << "Teknofest login successful for team" << teamId;
    emit loginSucceeded(teamId);
}

void TeknofestClient::onTelemetryResponse(const QByteArray &responseData, const QString &errorString) {
    QJsonDocument jsonResponse = QJsonDocument::fromJson(responseData);
    QJsonObject jsonObject = jsonResponse.object();
    QJsonArray planeArray = jsonObject["konumBilgileri"].toArray();
    emit telemetryReceived(planeArray);
}
