//
// Created by kardasland on 6/27/25.
//

#include "Vehicle.h"

Vehicle::Vehicle(int vehicleId, DataSource dataSource, QObject *parent) : QObject{parent}, m_vehicleId(vehicleId), m_dataSource(dataSource),
    m_systemId(0), m_teamId(0){
    qRegisterMetaType<Vehicle>("Vehicle");
    qRegisterMetaType<DataSource>("Vehicle::DataSource");
    qRegisterMetaType<int32_t>("int32_t");
}

int Vehicle::vehicleId() const {
    return m_vehicleId;
}

Vehicle::DataSource Vehicle::dataSource() const {
    return m_dataSource;
}

int Vehicle::systemId() const {
    return m_systemId;
}

int Vehicle::teamId() const {
    return m_teamId;
}

bool Vehicle::isSelected() const {
    return m_isSelected;
}

QGeoCoordinate Vehicle::coordinate() const {
    return m_coordinate;
}

double Vehicle::altitude() const {
    return m_altitude;
}

double Vehicle::groundSpeed() const {
    return m_groundSpeed;
}

double Vehicle::heading() const {
    return m_groundSpeed;
}

double Vehicle::roll() const {
    return m_roll;
}

double Vehicle::pitch() const {
    return m_pitch;
}

double Vehicle::batteryVoltage() const {
    return m_batteryVoltage;
}

bool Vehicle::isArmed() const {
    return m_isArmed;
}

QString Vehicle::flightMode() const {
    return m_flightMode;
}

QString Vehicle::displayId() const {
    return m_vehicleId == -1 ? QString() : QString::number(m_vehicleId);
}

QString Vehicle::altitudeString() const {
    return QString::number(m_altitude, 'f', 2) + " m";
}

QString Vehicle::groundSpeedString() const {
    return QString::number(m_groundSpeed, 'f', 2) + " m/s";
}

void Vehicle::setSystemId(const int id) {
    if (m_systemId == id) {
        return;
    }
    m_systemId = id;
    emit systemIdChanged();
    emit displayIdChanged();
}

void Vehicle::setTeamId(const int id) {
    if (m_teamId == id) {
        return;
    }
    m_teamId = id;
    emit teamIdChanged();
}

void Vehicle::setIsSelected(bool selected) {
    if (m_isSelected == selected) {
        return;
    }
    m_isSelected = selected;
    emit isSelectedChanged();
}

void Vehicle::setCoordinate(const QGeoCoordinate &coordinate) {
    if (m_coordinate == coordinate) {
        return;
    }
    m_coordinate = coordinate;
    emit coordinateChanged();
}

void Vehicle::setAltitude(double altitude) {
    if (m_altitude == altitude) {
        return;
    }
    m_altitude = altitude;
    emit altitudeChanged();
}

void Vehicle::setGroundSpeed(double speed) {
    if (m_groundSpeed == speed) {
        return;
    }
    m_groundSpeed = speed;
    emit groundSpeedChanged();
}

void Vehicle::setHeading(double heading) {
    if (m_heading == heading) {
        return;
    }
    m_heading = heading;
    emit headingChanged();
}

void Vehicle::setRoll(double roll) {
    if (m_roll == roll) {
        return;
    }
    m_roll = roll;
    emit rollChanged();
}

void Vehicle::setPitch(double pitch) {
    if (m_pitch == pitch) {
        return;
    }
    m_pitch = pitch;
    emit pitchChanged();
}

void Vehicle::setBatteryVoltage(double voltage) {
    if (qFuzzyCompare(m_batteryVoltage, voltage)) {
        return;
    }
    m_batteryVoltage = voltage;
    emit batteryVoltageChanged();
}

void Vehicle::setIsArmed(bool armed) {
    if (m_isArmed == armed) {
        return;
    }
    m_isArmed = armed;
    emit isArmedChanged();
}

void Vehicle::setFlightMode(const QString &mode) {
    if (m_flightMode == mode) {
        return;
    }
    m_flightMode = mode;
    emit flightModeChanged();
}
