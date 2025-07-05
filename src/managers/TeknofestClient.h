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

class TeknofestClient : public QObject
{
    Q_OBJECT

public:

    struct TeknofestAuthProperty {
        QString username;
        QString password;
    };

    struct TeknofestQRCodeProperty {
        double qrLatitude;
        double qrLongitude;

        [[nodiscard]] double qr_latitude() const {
            return qrLatitude;
        }

        [[nodiscard]] double qr_longitude() const {
            return qrLongitude;
        }
    };

    struct TeknofestServerProperties {
        TeknofestAuthProperty teknofestAuthProperty;
        TeknofestQRCodeProperty teknofestQRCodeProperty;
        QString url;
        QString takimid;
        QByteArray session_id;
        std::vector<int> planeIds; // List of team IDs
        void set_teknofest_auth_property(const TeknofestAuthProperty &teknofest_auth_property) {
            teknofestAuthProperty = teknofest_auth_property;
        }

        void set_teknofest_qr_code_property(const TeknofestQRCodeProperty &teknofest_qr_code_property) {
            teknofestQRCodeProperty = teknofest_qr_code_property;
        }

        void set_url(const QString &url) {
            this->url = url;
        }

        void set_takimid(const QString &takimid) {
            this->takimid = takimid;
        }

        void set_session_id(const QByteArray &session_id) {
            this->session_id = session_id;
        }

        void set_plane_ids(const std::vector<int> &plane_ids) {
            planeIds = plane_ids;
        }


    };

    [[nodiscard]] TeknofestServerProperties getServerProperties() {
        return m_serverProperties;
    }

    Q_INVOKABLE QString getTeknofestServerProperty_url() const {
        return m_serverProperties.url;
    }
    Q_INVOKABLE QString getTeknofestServerProperty_takimid() const {
        return m_serverProperties.takimid;
    }
    Q_INVOKABLE QByteArray getTeknofestServerProperty_session_id() const {
        return m_serverProperties.session_id;
    }
    Q_INVOKABLE QString getTeknofestAuthProperty_username() const {
        return m_serverProperties.teknofestAuthProperty.username;
    }
    Q_INVOKABLE QString getTeknofestAuthProperty_password() const {
        return m_serverProperties.teknofestAuthProperty.password;
    }
    Q_INVOKABLE double getTeknofestQRCodeProperty_latitude() const {
        return m_serverProperties.teknofestQRCodeProperty.qrLatitude;
    }
    Q_INVOKABLE double getTeknofestQRCodeProperty_longitude() const {
        return m_serverProperties.teknofestQRCodeProperty.qrLongitude;
    }
    Q_INVOKABLE QString getUrl() const {
        return m_serverProperties.url;
    }
    Q_INVOKABLE QString getTakimid() const {
        return m_serverProperties.takimid;
    }
    Q_INVOKABLE QByteArray getSessionId() const {
        return m_serverProperties.session_id;
    }


    void setServerProperties(const TeknofestServerProperties &serverProperties) {
        m_serverProperties = serverProperties;
    }

    explicit TeknofestClient(QObject *parent = nullptr);

public slots:
    Q_INVOKABLE void login() const;
    Q_INVOKABLE void startFetchingTelemetry();
    Q_INVOKABLE void stopFetchingTelemetry();
    void transmitTelemetryRequest(QJsonObject &vehicleData);

    signals:
        void loginSucceeded(int teamid);
        void loginFailed(const QString& error);
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
    TeknofestServerProperties m_serverProperties;
    HttpClient* m_httpClient;
};



#endif //TEKNOFESTMANAGER_H
