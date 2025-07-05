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
    m_mavlinkManager->connectUdp("127.0.1", 14550);
}

void AppContext::initializeTeknofestServer() {
    // set the Teknofest server properties
    TeknofestClient::TeknofestAuthProperty authProperty;
    authProperty.username = "takimkadi1";
    authProperty.password = "takimsifresi1";
    TeknofestClient::TeknofestQRCodeProperty qrCodeProperty;
    qrCodeProperty.qrLatitude = 40.0;
    qrCodeProperty.qrLongitude = 29.0;
    TeknofestClient::TeknofestServerProperties serverProperties;
    serverProperties.set_teknofest_auth_property(authProperty);
    serverProperties.set_teknofest_qr_code_property(qrCodeProperty);
    serverProperties.set_url(getenv("TEKNOFEST_API_URL"));
    // serverProperties.set_takimid("123"); // Replace with actual team ID
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
    connect(m_teknofestClient, &TeknofestClient::loginSucceeded,
            m_vehicleManager, &VehicleManager::startTransmittingTelemetry);
}
