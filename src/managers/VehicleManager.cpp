#include "VehicleManager.h"
#include <QJsonArray>
#include <QJsonObject>
#include <QJsonValue>
#include <QDebug>
#include <QSet>

void VehicleManager::append_vehicle(QQmlListProperty<Vehicle>* list, Vehicle* vehicle) {
    Q_UNUSED(list);
    Q_UNUSED(vehicle);
}

qsizetype VehicleManager::vehicle_count(QQmlListProperty<Vehicle>* list) {
    // Cast the generic list object back to our manager to access the member variable.
    VehicleManager* manager = qobject_cast<VehicleManager*>(list->object);
    return manager ? manager->m_vehicleList.count() : 0;
}

Vehicle* VehicleManager::vehicle_at(QQmlListProperty<Vehicle>* list, qsizetype index) {
    VehicleManager* manager = qobject_cast<VehicleManager*>(list->object);
    if (manager && index >= 0 && index < manager->m_vehicleList.count()) {
        return manager->m_vehicleList.at(index);
    }
    return nullptr;
}


// --- Class Implementation ---

VehicleManager::VehicleManager(QObject *parent) : QObject(parent) {
    // No registration needed here for the list property itself.
}

// The getter for the QML property. It creates the QQmlListProperty "proxy" object.
QQmlListProperty<Vehicle> VehicleManager::vehicles() {
    return QQmlListProperty<Vehicle>(this,
        nullptr, // Data pointer (optional, we use list->object)
        &VehicleManager::append_vehicle, // <<< PROVIDE THE FUNCTION HERE
        &VehicleManager::vehicle_count,
        &VehicleManager::vehicle_at,
        nullptr, // clear
        nullptr, // replace
        nullptr  // removeLast
    );
}

Vehicle* VehicleManager::selectedVehicle() const {
    return m_selectedVehicle;
}

Vehicle * VehicleManager::getMainVehicle() const{
    // Return the first MAVLink vehicle as the main vehicle.
    for (Vehicle* vehicle : m_vehicleList) {
        if (vehicle->dataSource() == Vehicle::LocalMavlink) {
            // qDebug() << "Main vehicle found with ID:" << vehicle->vehicleId();
            return vehicle;
        }
    }
    return nullptr; // No MAVLink vehicle found
}

void VehicleManager::updateMavlinkVehicle(int systemId, const QGeoCoordinate& coord, double alt, double relativeAltitude,
                                          double speed, double heading, double roll, double pitch,
                                          double batteryRemaining, double batteryVoltage, double batteryCurrent,  std::optional<bool> isArmed, const QString& flightMode)
{
    Vehicle* vehicle = getOrCreateMavlinkVehicle(systemId);

    // Update the vehicle's properties, but only if the new data is valid (not NaN).
    // This handles the case where different MAVLink messages arrive at different times.
    if (coord.isValid()) {
        vehicle->setCoordinate(coord);
    }
    if (!std::isnan(alt)) {
        vehicle->setAltitude(alt);
    }
    if (!std::isnan(relativeAltitude)) {
        vehicle->setRelativeAltitude(relativeAltitude);
    }
    if (!std::isnan(speed)) {
        vehicle->setGroundSpeed(speed);
    }
    if (!std::isnan(heading)) {
        vehicle->setHeading(heading);
    }
    if (!std::isnan(roll)) {
        vehicle->setRoll(roll);
    }
    if (!std::isnan(pitch)) {
        vehicle->setPitch(pitch);
    }
    if (!std::isnan(batteryRemaining)) {
        vehicle->setBatteryRemaining(batteryRemaining);
    }
    if (!std::isnan(batteryVoltage)) {
        vehicle->setBatteryVoltage(batteryVoltage);
    }
    if (!std::isnan(batteryCurrent)) {
        vehicle->setBatteryCurrent(batteryCurrent);
    }
    if (!flightMode.isEmpty() && flightMode != "Unknown") {
        vehicle->setFlightMode(flightMode);
    }

    // isArmed is a boolean, so it's always valid.
    vehicle->setOptionalArmed(isArmed);

    // emit vehiclesChanged() ; // Notify QML that the list has changed
}

void VehicleManager::updateTeknofestVehicles(const QJsonArray &vehicleData) {
    QSet<int> receivedTeamIds; // Keep track of which vehicles we received in this update.

    for (const QJsonValue &value : vehicleData) {
        if (!value.isObject()) continue;

        QJsonObject obj = value.toObject();
        int teamId = obj.value("takim_numarasi").toInt(-1);
        if (teamId == -1) continue;

        receivedTeamIds.insert(teamId);
        Vehicle* vehicle = getOrCreateTeknofestVehicle(teamId);

        // Update properties from the Teknofest API JSON structure
        vehicle->setCoordinate(QGeoCoordinate(obj.value("iha_enlem").toDouble(), obj.value("iha_boylam").toDouble()));
        vehicle->setAltitude(obj.value("iha_irtifa").toDouble());
        vehicle->setGroundSpeed(obj.value("iha_hiz").toDouble());
        vehicle->setHeading(obj.value("iha_yonelme").toDouble());
        vehicle->setRoll(obj.value("iha_yatis").toDouble());
        vehicle->setPitch(obj.value("iha_dikilme").toDouble());
    }

    // --- Logic to remove stale vehicles that are no longer in the API feed ---
    QList<int> teamsToRemove;
    for (int teamId : m_teknofestIdToVehicleId.keys()) {
        if (!receivedTeamIds.contains(teamId)) {
            teamsToRemove.append(teamId);
        }
    }

    if (!teamsToRemove.isEmpty()) {
        for (int teamId : teamsToRemove) {
            int vehicleIdToRemove = m_teknofestIdToVehicleId.take(teamId);
            Vehicle* vehicleToRemove = m_vehicleMap.take(vehicleIdToRemove);

            if (vehicleToRemove) {
                qDebug() << "Removing stale Teknofest vehicle, Team ID:" << teamId;
                if (m_selectedVehicle == vehicleToRemove) {
                    selectVehicle(-1); // Deselect if it was the selected one
                }
                m_vehicleList.removeAll(vehicleToRemove); // Remove from the QML list
                vehicleToRemove->deleteLater();          // Safely delete the object
            }
        }
        emit vehiclesChanged(); // Notify QML that the list has shrunk
    }
}



void VehicleManager::startTransmittingTelemetry(int teamid) {
    qDebug() << "Starting telemetry transmission timer";
    m_telemetryTimer = new QTimer(this);
    Vehicle* firstMavlinkVehicle = nullptr;
    for (Vehicle* vehicle : m_vehicleList) {
        if (vehicle->dataSource() == Vehicle::LocalMavlink) {
            firstMavlinkVehicle = vehicle;
            break;
        }
    }
    if (!firstMavlinkVehicle) {
        qWarning() << "No MAVLink vehicle found to transmit telemetry.";
        return;
    }
    qDebug() << "Setting team ID for first MAVLink vehicle:" << firstMavlinkVehicle->vehicleId() << "to" << teamid;
    firstMavlinkVehicle->setTeamId(teamid);
    emit vehiclesChanged();
    connect(m_telemetryTimer, &QTimer::timeout, this, [this]() {

        // get first mavlink vehicle data.
        Vehicle* firstMavlinkVehicle = nullptr;
        for (Vehicle* vehicle : m_vehicleList) {
            if (vehicle->dataSource() == Vehicle::LocalMavlink) {
                firstMavlinkVehicle = vehicle;
                break;
            }
        }
        // qDebug() << "Transmitting telemetry for vehicle:" << (firstMavlinkVehicle ? firstMavlinkVehicle->vehicleId() : -1);


        QJsonObject vehicleData;
        /**
        *{
        "takim_numarasi": 1,
        "iha_enlem": 41.508775,
        "iha_boylam": 36.118335,
        "iha_irtifa": 38,
        "iha_dikilme": 7,
        "iha_yonelme": 210,
        "iha_yatis": -30,
        "iha_hiz": 28,
        "iha_batarya": 50,
        "iha_otonom": 1,
        "iha_kilitlenme": 0, // Set to 1 if locking, then include hedef_merkez_X/Y etc.
        "gps_saati": {
        "saat": 11,
        "dakika": 38,
        "saniye": 37,
        "milisaniye": 654
        }
        }**/
        vehicleData["takim_numarasi"] = firstMavlinkVehicle->teamId();
        vehicleData["iha_enlem"] = firstMavlinkVehicle->coordinate().latitude();
        vehicleData["iha_boylam"] = firstMavlinkVehicle->coordinate().longitude();
        vehicleData["iha_irtifa"] = firstMavlinkVehicle->altitude();
        vehicleData["iha_hiz"] = firstMavlinkVehicle->groundSpeed();
        vehicleData["iha_yonelme"] = firstMavlinkVehicle->heading();
        vehicleData["iha_yatis"] = firstMavlinkVehicle->roll();
        vehicleData["iha_dikilme"] = firstMavlinkVehicle->pitch();
        vehicleData["iha_batarya"] = firstMavlinkVehicle->batteryRemaining();
        vehicleData["iha_otonom"] = 1; // Assuming always autonomous for now
        vehicleData["iha_kilitlenme"] = 0; // Assuming not locked, set to 1 if locking
        // Add GPS time if available
        QDateTime gpsTime = QDateTime::currentDateTimeUtc(); // Use current time for simplicity
        QJsonObject gpsTimeObj;
        gpsTimeObj["saat"] = gpsTime.time().hour();
        gpsTimeObj["dakika"] = gpsTime.time().minute();
        gpsTimeObj["saniye"] = gpsTime.time().second();
        gpsTimeObj["milisaniye"] = gpsTime.time().msec();
        vehicleData["gps_saati"] = gpsTimeObj;
        // Emit the signal to transmit telemetry data
        emit transmitTelemetryRequest(vehicleData);
        emit hssCoordinateRequest();

    });
    m_telemetryTimer->start(1000); // Transmit every 5 seconds
}

void VehicleManager::stopTransmittingTelemetry() {
    qDebug() << "Stopping telemetry transmission timer";
    if (m_telemetryTimer) {
        disconnect(m_telemetryTimer, &QTimer::timeout, this, nullptr);
        m_telemetryTimer->stop();
        m_telemetryTimer->deleteLater();
        m_telemetryTimer = nullptr;
    }
    emit vehiclesChanged(); // Notify QML that the list has changed
}

void VehicleManager::selectVehicle(int vehicleId) {
    Vehicle* vehicleToSelect = m_vehicleMap.value(vehicleId, nullptr);

    for (Vehicle* vehicle : m_vehicleList) {
        qDebug() << "Checking vehicle ID:" << vehicle->vehicleId()
                 << "System ID:" << vehicle->systemId()
                 << "Team ID:" << vehicle->teamId();
    }

    qDebug() << "Selecting vehicle with ID:" << vehicleId
             << "Found:" << (vehicleToSelect ? "Yes" : "No");
    if (m_selectedVehicle == vehicleToSelect) {
        // toggle selection if the same vehicle is selected again
        qDebug() << "Vehicle with ID:" << vehicleId << "is already selected, toggling selection off.";
        vehicleToSelect->setIsSelected(false);
        m_selectedVehicle = nullptr; // Deselect the vehicle
        emit selectedVehicleChanged(); // Notify QML that the selection has changed
        return; // No need to continue if we just deselected the same vehicle
    }

    // Deselect the previously selected vehicle, if there was one.
    if (m_selectedVehicle) {
        m_selectedVehicle->setIsSelected(false);
    }

    m_selectedVehicle = vehicleToSelect;

    // Select the new one, if it's valid.
    if (m_selectedVehicle) {
        m_selectedVehicle->setIsSelected(true);
    }

    qDebug() << "New selected vehicle ID:" << (m_selectedVehicle ? m_selectedVehicle->vehicleId() : -1);

    emit vehiclesChanged(); // Notify QML that the list has changed
    emit selectedVehicleChanged();
}

void VehicleManager::clearTeknofestVehicles() {
    qDebug() << "Clearing all Teknofest vehicles";
    QList<int> teamIds = m_teknofestIdToVehicleId.keys();
    for (int teamId : teamIds) {
        int vehicleId = m_teknofestIdToVehicleId.take(teamId);
        Vehicle* vehicle = m_vehicleMap.take(vehicleId);
        if (vehicle) {
            qDebug() << "Removing Teknofest vehicle, Team ID:" << teamId
                     << "Vehicle ID:" << vehicle->vehicleId();
            if (m_selectedVehicle == vehicle) {
                selectVehicle(-1); // Deselect if it was the selected one
            }
            m_vehicleList.removeAll(vehicle); // Remove from the QML list
            vehicle->deleteLater(); // Safely delete the object
        }
    }
    emit vehiclesChanged(); // Notify QML that the list has shrunk
}

Vehicle* VehicleManager::getOrCreateMavlinkVehicle(int systemId) {
    if (m_mavlinkIdToVehicleId.contains(systemId)) {
        int vehicleId = m_mavlinkIdToVehicleId.value(systemId);
        return m_vehicleMap.value(vehicleId);
    }

    qDebug() << "Creating new MAVLink vehicle, System ID:" << systemId;
    int newVehicleId = m_nextVehicleId++;
    Vehicle* newVehicle = new Vehicle(newVehicleId, Vehicle::LocalMavlink, this);
    newVehicle->setSystemId(systemId);

    // Add the new vehicle to our data structures
    m_vehicleMap.insert(newVehicleId, newVehicle);
    m_vehicleList.append(newVehicle);
    m_mavlinkIdToVehicleId.insert(systemId, newVehicleId);

    // Notify QML that the model list has grown
    emit vehiclesChanged();
    return newVehicle;
}

Vehicle* VehicleManager::getOrCreateTeknofestVehicle(int teamId) {
    if (m_teknofestIdToVehicleId.contains(teamId)) {
        int vehicleId = m_teknofestIdToVehicleId.value(teamId);
        return m_vehicleMap.value(vehicleId);
    }

    qDebug() << "Creating new Teknofest vehicle, Team ID:" << teamId;
    int newVehicleId = m_nextVehicleId++;
    Vehicle* newVehicle = new Vehicle(newVehicleId, Vehicle::TeknofestAPI, this);
    newVehicle->setTeamId(teamId);

    // Add the new vehicle to our data structures
    m_vehicleMap.insert(newVehicleId, newVehicle);
    m_vehicleList.append(newVehicle);
    m_teknofestIdToVehicleId.insert(teamId, newVehicleId);

    // Notify QML that the model list has grown
    emit vehiclesChanged();
    return newVehicle;
}