//
// Created by kardasland on 6/27/25.
//

#ifndef TEKNOFESTMANAGER_H
#define TEKNOFESTMANAGER_H

#include <QNetworkAccessManager>
#include <QObject>
#include <QJsonArray>
#include <QTimer>

#include "src/network/HttpClient.h"
#include "src/teknofest/TeknofestHSSProperty.h"
#include "src/teknofest/TeknofestServerProperty.h"

class TeknofestClient : public QObject
{
    Q_OBJECT
    Q_PROPERTY(bool loggedIn READ isLoggedIn WRITE setLoggedIn NOTIFY loggedInChanged)
    Q_PROPERTY(QString statusString READ statusString WRITE setStatusString NOTIFY statusStringChanged)
    Q_PROPERTY(TeknofestServerProperties* serverProperties READ getServerProperties WRITE setServerProperties NOTIFY serverPropertiesChanged)

public:

    Q_INVOKABLE bool isLoggedIn() const {
        return m_loggedIn;
    }

    void setLoggedIn(bool loggedIn) {
        if (m_loggedIn == loggedIn) return; // No change
        m_loggedIn = loggedIn;
        emit loggedInChanged();
    }

    TeknofestServerProperties* getServerProperties() const {
        return m_serverProperties;
    }

    void setServerProperties(TeknofestServerProperties* serverProperties) {
        if (m_serverProperties == serverProperties) return; // No change
        m_serverProperties = serverProperties;
        emit serverPropertiesChanged();
    }

    Q_INVOKABLE QString statusString() const {
        return m_statusString;
    }
    void setStatusString(const QString &statusString) {
        if (m_statusString == statusString) return; // No change
        m_statusString = statusString;
        emit statusStringChanged();
    }
    Q_INVOKABLE void disconnectFromServer();
    explicit TeknofestClient(QObject *parent = nullptr);

public slots:
    Q_INVOKABLE void login() const;

    Q_INVOKABLE void startFetchingTelemetry();
    Q_INVOKABLE void stopFetchingTelemetry();
    void statusChanged(const QString& status);
    void transmitTelemetryRequest(QJsonObject &vehicleData);
    void hssResponseReceived(const QByteArray& responseData, const QString& errorString);
    void hssCoordinateRequest();
signals:
    void loginSucceeded(int teamid);
    void loginFailed(const QString& error);
    void loggedInChanged();
    void statusStringChanged();
    void serverPropertiesChanged();
    // This signal is connected to the VehicleManager
    void telemetryReceived(const QJsonArray &vehicleData);

private slots:
    void fetchTelemetry();
    void onLoginFinished(int teamId, const QByteArray& sessionId, const QString& errorString);
    void onTelemetryResponse(const QByteArray& responseData, const QString& errorString);

private:
    QNetworkAccessManager m_networkManager;
    QTimer* m_telemetryTimer = nullptr;
    QString m_apiUrl;
    QByteArray m_sessionId;
    bool m_loggedIn = false;
    QString m_statusString;
    TeknofestServerProperties* m_serverProperties;
    HttpClient* m_httpClient;
};



#endif //TEKNOFESTMANAGER_H
