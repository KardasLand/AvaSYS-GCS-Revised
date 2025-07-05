//
// Created by kardasland on 6/27/25.
//

#include "AppContext.h"

AppContext * AppContext::instance() {
    static AppContext instance;
    return &instance;
}

VehicleManager * AppContext::vehicleManager() const {
    return m_vehicleManager;
}

MavlinkManager * AppContext::mavlinkManager() const {
    return m_mavlinkManager;
}

TeknofestClient * AppContext::teknofestClient() const {
    return m_teknofestClient;
}

void AppContext::autoConnectMavlink() {
    // start a mavlink connection
    // m_mavlinkManager->connectUdp("127.0.1", 14550);
    m_mavlinkManager->setHost("127.0.1");
    m_mavlinkManager->setPort(14550);
    m_mavlinkManager->connectFromSettings();
}

void AppContext::initializeTeknofestServer() {
    TeknofestServerProperties *serverProperties = new TeknofestServerProperties();
    TeknofestAuthProperty *authProperty = new TeknofestAuthProperty();
    TeknofestQRCodeProperty *qrCodeProperty = new TeknofestQRCodeProperty();


    serverProperties->setTeknofestAuthProperty(authProperty);
    serverProperties->setTeknofestQRCodeProperty(qrCodeProperty);
    serverProperties->setUrl(getenv("TEKNOFEST_API_URL"));

    QString username = "takimkadi1";
    QString password = "takimsifresi1";
    authProperty->setUsername(username);
    authProperty->setPassword(password);

    // Set default QR code coordinates (can be updated later)
    qrCodeProperty->setQrLatitude(40.0); // Example latitude
    qrCodeProperty->setQrLongitude(29.0); // Example longitude

    m_teknofestClient->setServerProperties(serverProperties);
    m_teknofestClient->login();
}

AppContext::AppContext(QObject *parent) : QObject{parent},
                                          m_vehicleManager(new VehicleManager(this)),
                                          m_mavlinkManager(new MavlinkManager(this)),
                                          m_teknofestClient(new TeknofestClient(this)) {

    connect(m_mavlinkManager, &MavlinkManager::mavlinkVehicleUpdated,
            m_vehicleManager, &VehicleManager::updateMavlinkVehicle);
    connect(m_teknofestClient, &TeknofestClient::telemetryReceived,
            m_vehicleManager, &VehicleManager::updateTeknofestVehicles);
    connect(m_vehicleManager, &VehicleManager::transmitTelemetryRequest,
        m_teknofestClient, &TeknofestClient::transmitTelemetryRequest);
    connect(m_vehicleManager, &VehicleManager::hssCoordinateRequest,
        m_teknofestClient, &TeknofestClient::hssCoordinateRequest);
    connect(m_teknofestClient, &TeknofestClient::loginSucceeded,
            m_vehicleManager, &VehicleManager::startTransmittingTelemetry);
}
