//
// Created by kardasland on 6/27/25.
//

#include "TeknofestClient.h"

#include <utility>

#include "src/core/AppContext.h"

TeknofestClient::TeknofestClient(QObject *parent) : QObject{parent} {
    qRegisterMetaType<QJsonArray>("QJsonArray");
    m_httpClient = new HttpClient(this);
    m_serverProperties = new TeknofestServerProperties(this);

    connect(m_httpClient, &HttpClient::loginFinished, this, &TeknofestClient::onLoginFinished);
    connect(m_httpClient, &HttpClient::telemetryResponseReceived, this, &TeknofestClient::onTelemetryResponse);
    connect(m_httpClient, &HttpClient::statusChanged, this, &TeknofestClient::statusChanged);
    connect(m_httpClient, &HttpClient::hssResponseReceived, this, &TeknofestClient::hssResponseReceived);
}

void TeknofestClient::login() const {
    qDebug() << "Method fired TeknofestClient::login()";
    qDebug() << "Info - TeknofestClient::login() - URL: " << m_serverProperties->getUrl();
    qDebug() << "Info - TeknofestClient::login() - Username: " << m_serverProperties->getTeknofestAuthProperty()->getUsername();
    qDebug() << "Info - TeknofestClient::login() - Password: " << m_serverProperties->getTeknofestAuthProperty()->getPassword();
    m_httpClient->sendLoginRequest(
            m_serverProperties->getUrl() + "/giris",
            m_serverProperties->getTeknofestAuthProperty()->getUsername(),
            m_serverProperties->getTeknofestAuthProperty()->getPassword()
        );
}

void TeknofestClient::disconnectFromServer() {
    qDebug() << "Method fired TeknofestClient::disconnectFromServer()";
    // remove all teknofest planes from VehicleManager before disconnecting
    AppContext::instance()->vehicleManager()->stopTransmittingTelemetry();
    AppContext::instance()->vehicleManager()->clearTeknofestVehicles();
    // m_httpClient->disconnectClient();
    setLoggedIn(false);
    setStatusString("Disconnected from Teknofest server.");
}

void TeknofestClient::startFetchingTelemetry() {
}

void TeknofestClient::stopFetchingTelemetry() {
}

void TeknofestClient::statusChanged(const QString &status) {
    QString lowerStatus = status.toLower();

    if (lowerStatus == "loginrequestsent") {
        setStatusString("Login request sent to Teknofest server.");
    } else if (lowerStatus == "loginreplyerror") {
        setStatusString("Login reply error from Teknofest server.");
    } else if (lowerStatus == "invalid url") {
        setStatusString("Invalid URL for Teknofest server.");
    } else if (lowerStatus == "telemetryreplyerror") {
        setStatusString("Telemetry reply error from Teknofest server.");
    } else if (status == "TelemetryReplySuccess") { // Case-sensitive match
        setStatusString("Connected.");
    } else if (lowerStatus == "hssrequestsent") {
        setStatusString("Connected.");
    } else if (lowerStatus == "hssreplyerror") {
        setStatusString("HSS reply error from Teknofest server.");
    } else if (lowerStatus == "hssreplysuccess") {
        setStatusString("Connected.");
    } else if (lowerStatus == "disconnected") {
        setStatusString("Disconnected from Teknofest server.");
    }
    else {
        setStatusString("Unknown status: " + status);
    }
}

void TeknofestClient::transmitTelemetryRequest(QJsonObject &vehicleData) {
    m_httpClient->sendTelemetryRequest(
        m_serverProperties->getUrl() + "/telemetri_gonder",
        m_sessionId,
        vehicleData
    );
}

// todo not efficient enough
void TeknofestClient::hssResponseReceived(const QByteArray &responseData, const QString &errorString) {
    if (!errorString.isEmpty()) {
        qWarning() << "HSS response error:" << errorString;
        return;
    }
    QJsonDocument jsonResponse = QJsonDocument::fromJson(responseData);
    QJsonObject jsonObject = jsonResponse.object();
    // qDebug() << "HSS response received:" << jsonObject;
    QJsonArray hssArray = jsonObject["hss_koordinat_bilgileri"].toArray();
    m_serverProperties->getTeknofestHSSProperties().clear();
    int i = 0;
    QList<TeknofestHSSProperty*> hssProperties = QList<TeknofestHSSProperty*>();
    for (const QJsonValue &value : hssArray) {
        QJsonObject hssObject = value.toObject();
        TeknofestHSSProperty* hssProperty = new TeknofestHSSProperty();
        hssProperty->setHssLatitude(hssObject["hssEnlem"].toDouble());
        hssProperty->setHssLongitude(hssObject["hssBoylam"].toDouble());
        hssProperty->setHssCircleRadius(hssObject["hssYaricap"].toDouble());
        // qDebug() << "HSS Property" << i << ":"
        //          << "Latitude:" << hssProperty->hssLatitude()
        //          << "Longitude:" << hssProperty->hssLongitude()
        //          << "Radius:" << hssProperty->hssCircleRadius();
        hssProperties.append(hssProperty);
        i++;
    }
    m_serverProperties->setTeknofestHSSProperties(hssProperties);
    // qDebug() << "Teknofest HSS properties updated, count:" << m_serverProperties->getTeknofestHSSProperties().count();

}

void TeknofestClient::hssCoordinateRequest() {
    m_httpClient->hssRequest(
        m_serverProperties->getUrl() + "/hss_koordinatlari",
        m_serverProperties->getSessionId()
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
    m_serverProperties->setTakimid(QString::number(teamId));
    m_serverProperties->getSessionId() = sessionId;
    qDebug() << "Teknofest login successful for team" << teamId;
    setLoggedIn(true);
    emit loginSucceeded(teamId);
}

void TeknofestClient::onTelemetryResponse(const QByteArray &responseData, const QString &errorString) {
    QJsonDocument jsonResponse = QJsonDocument::fromJson(responseData);
    QJsonObject jsonObject = jsonResponse.object();
    QJsonArray planeArray = jsonObject["konumBilgileri"].toArray();
    emit telemetryReceived(planeArray);
}
