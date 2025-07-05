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

    Q_INVOKABLE void setTeknofestServerSettings(const QString& host) {
        m_teknofestClient->getServerProperties()->setUrl(host);
    }



    Q_INVOKABLE void setTeknofestAuth(QString& username, QString& password) {
        TeknofestAuthProperty* authProperty = m_teknofestClient->getServerProperties()->getTeknofestAuthProperty();
        if (!authProperty) {
            authProperty = new TeknofestAuthProperty(this);
            m_teknofestClient->getServerProperties()->setTeknofestAuthProperty(authProperty);
        }
        authProperty->setUsername(username);
        authProperty->setPassword(password);
    }

    Q_INVOKABLE void setTeknofestQRCode(double latitude, double longitude) {
        TeknofestQRCodeProperty* qrCodeProperty = m_teknofestClient->getServerProperties()->getTeknofestQRCodeProperty();
        if (!qrCodeProperty) {
            qrCodeProperty = new TeknofestQRCodeProperty(this);
            m_teknofestClient->getServerProperties()->setTeknofestQRCodeProperty(qrCodeProperty);
        }
        qrCodeProperty->setQrLatitude(latitude);
        qrCodeProperty->setQrLongitude(longitude);
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
