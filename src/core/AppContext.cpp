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

AppContext::AppContext(QObject *parent) : QObject{parent},
    m_vehicleManager(new VehicleManager(this)),
    m_mavlinkManager(new MavlinkManager(this)),
    m_teknofestClient(new TeknofestClient(this)) {
}
