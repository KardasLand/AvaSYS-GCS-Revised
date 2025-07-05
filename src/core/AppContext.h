//
// Created by kardasland on 6/27/25.
//

#ifndef APPCONTEXT_H
#define APPCONTEXT_H

#include "../managers/VehicleManager.h"
#include "../managers/MavlinkManager.h"
#include "../managers/TeknofestClient.h"
#include <QObject>

class AppContext : public QObject
{
    Q_OBJECT
    Q_PROPERTY(VehicleManager* vehicleManager READ vehicleManager CONSTANT)
    Q_PROPERTY(MavlinkManager* mavlinkManager READ mavlinkManager CONSTANT)
    Q_PROPERTY(TeknofestClient* teknofestClient READ teknofestClient CONSTANT)

public:
    static AppContext* instance();

    VehicleManager* vehicleManager() const;
    MavlinkManager* mavlinkManager() const;
    TeknofestClient* teknofestClient() const;
    void autoConnectMavlink();
    void initializeTeknofestServer();

    Q_INVOKABLE void startTeknofestConnection() {
        m_teknofestClient->login();
    }

    Q_INVOKABLE void setMavlinkSettings(const QString& host, int port) {
        m_mavlinkManager->connectUdp(host, port);
    }

    Q_INVOKABLE void setTeknofestServerSettings(const QString& host, int port) {
        TeknofestClient::TeknofestServerProperties serverProperties;
        // check if port is null, if so dont set it
        if (std::isnan(port)) {
            serverProperties.set_url(host);
        } else {
            serverProperties.set_url(QString("%1:%2").arg(host).arg(port));
        }
        m_teknofestClient->setServerProperties(serverProperties);
    }

    Q_INVOKABLE void setTeknofestAuth(const QString& username, const QString& password) {
        TeknofestClient::TeknofestAuthProperty authProperty;
        authProperty.username = username;
        authProperty.password = password;
        m_teknofestClient->getServerProperties().set_teknofest_auth_property(authProperty);
    }

    Q_INVOKABLE void setTeknofestQRCode(double latitude, double longitude) {
        TeknofestClient::TeknofestQRCodeProperty qrCodeProperty;
        qrCodeProperty.qrLatitude = latitude;
        qrCodeProperty.qrLongitude = longitude;
        m_teknofestClient->getServerProperties().set_teknofest_qr_code_property(qrCodeProperty);
    }
private:
    explicit AppContext(QObject* parent = nullptr);
    AppContext(const AppContext&) = delete;
    AppContext& operator=(const AppContext&) = delete;

    VehicleManager* m_vehicleManager;
    MavlinkManager* m_mavlinkManager;
    TeknofestClient* m_teknofestClient;
};



#endif //APPCONTEXT_H
