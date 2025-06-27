//
// Created by kardasland on 6/27/25.
//

#ifndef TEKNOFESTMANAGER_H
#define TEKNOFESTMANAGER_H

#include <QNetworkAccessManager>
#include <QObject>
#include <QJsonArray>
#include <QTimer>

class TeknofestClient : public QObject
{
    Q_OBJECT

public:
    explicit TeknofestClient(QObject *parent = nullptr);

public slots:
    Q_INVOKABLE void login(const QString& url, const QString& username, const QString& password);
    Q_INVOKABLE void startFetchingTelemetry();
    Q_INVOKABLE void stopFetchingTelemetry();

    signals:
        void loginSucceeded(int teamId);
        void loginFailed(const QString& error);
        // This signal is connected to the VehicleManager
        void telemetryReceived(const QJsonArray &vehicleData);

private slots:
    void onLoginReply(QNetworkReply* reply);
    void onTelemetryReply(QNetworkReply* reply);
    void fetchTelemetry();

private:
    QNetworkAccessManager m_networkManager;
    QTimer* m_telemetryTimer = nullptr;
    QString m_apiUrl;
    QByteArray m_sessionId;
};



#endif //TEKNOFESTMANAGER_H
