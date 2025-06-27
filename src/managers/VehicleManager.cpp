// file: src/managers/VehicleManager.cpp

#include "VehicleManager.h"

// --- FIX IS HERE ---
#include <QJsonArray>
#include <QJsonObject>
#include <QJsonValue> // Also good practice to include this explicitly

VehicleManager::VehicleManager(QObject *parent) : QObject{parent} {
    qRegisterMetaType<int32_t>("int32_t");
    m_nextVehicleId = 0; // Initialize vehicle ID counter
}

QList<Vehicle *> VehicleManager::vehicles() const {
    return m_vehicles.values();
}

Vehicle * VehicleManager::selectedVehicle() const {
    return m_selectedVehicle;
}

void VehicleManager::updateMavlinkVehicle(int systemId, const QGeoCoordinate &coord, double alt, double speed,
    double heading, bool isArmed) {
    // Implementation needed here
}

void VehicleManager::updateTeknofestVehicles(const QJsonArray &vehicleData) {
    for (const QJsonValue &value : vehicleData) {
        if (!value.isObject()) continue;

        QJsonObject obj = value.toObject();
        int teamId = obj.value("teamId").toInt();
        Vehicle *vehicle = getOrCreateTeknofestVehicle(teamId);

        // Update vehicle properties from JSON
        // Note: Teknofest API seems to use 'takim_numarasi' for teamId and 'iha_enlem' etc. for telemetry.
        // You may need to adjust these keys based on the actual API spec.
        vehicle->setCoordinate(QGeoCoordinate(obj.value("latitude").toDouble(), obj.value("longitude").toDouble()));
        vehicle->setAltitude(obj.value("altitude").toDouble());
        vehicle->setGroundSpeed(obj.value("groundSpeed").toDouble());
        vehicle->setHeading(obj.value("heading").toDouble());

        // This is probably not in the telemetry, but depends on your app logic
        // vehicle->setIsSelected(obj.value("isSelected").toBool());
    }
    // Emit signal to notify about the updated vehicles *after* the loop
    emit vehiclesChanged();
}

void VehicleManager::selectVehicle(int vehicleId) {
    if (m_vehicles.contains(vehicleId)) {
        Vehicle *vehicle = m_vehicles[vehicleId];
        if (m_selectedVehicle != vehicle) {
            // Unselect the old one if it exists
            if(m_selectedVehicle) {
                m_selectedVehicle->setIsSelected(false);
            }
            // Select the new one
            m_selectedVehicle = vehicle;
            m_selectedVehicle->setIsSelected(true);

            emit selectedVehicleChanged();
        }
    } else if (m_selectedVehicle) { // If vehicleId is invalid, deselect
        m_selectedVehicle->setIsSelected(false);
        m_selectedVehicle = nullptr;
        emit selectedVehicleChanged();
    }
}

Vehicle * VehicleManager::getOrCreateMavlinkVehicle(int systemId) {
    // You should probably map by a unique property, like systemId for mavlink
    // Or have separate maps if IDs can overlap between sources.
    // For now, assuming systemId is unique key for mavlink.
    if (m_vehicles.contains(systemId)) {
        return m_vehicles[systemId];
    }

    // Create a new vehicle with a unique ID
    Vehicle *newVehicle = new Vehicle(m_nextVehicleId++, Vehicle::LocalMavlink, this);
    newVehicle->setSystemId(systemId);
    m_vehicles.insert(systemId, newVehicle);

    emit vehiclesChanged();
    return newVehicle;
}

Vehicle * VehicleManager::getOrCreateTeknofestVehicle(int teamId) {
    // For Teknofest, teamId might be the key
    if (m_vehicles.contains(teamId)) {
        return m_vehicles[teamId];
    }

    // Create a new vehicle with a unique ID
    Vehicle *newVehicle = new Vehicle(m_nextVehicleId++, Vehicle::TeknofestAPI, this);
    newVehicle->setTeamId(teamId);
    m_vehicles.insert(teamId, newVehicle);

    emit vehiclesChanged();
    return newVehicle;
}