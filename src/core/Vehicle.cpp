//
// Created by kardasland on 6/27/25.
//

#include "Vehicle.h"

Vehicle::Vehicle(int vehicleId, DataSource dataSource, QObject *parent) : QObject{parent}, m_vehicleId(vehicleId), m_dataSource(dataSource),
    m_systemId(-1), m_teamId(-1){
    qRegisterMetaType<Vehicle>("Vehicle");
    // qRegisterMetaType<DataSource>("Vehicle::DataSource");
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

double Vehicle::relativeAltitude() const {
    return m_relativeAltitude;
}

double Vehicle::groundSpeed() const {
    return m_groundSpeed;
}

double Vehicle::heading() const {
    return m_heading;
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

double Vehicle::batteryRemaining() const {
    return m_batteryRemaining;
}

double Vehicle::batteryCurrent() const {
    return m_batteryCurrent;
}

bool Vehicle::isArmed() const {
    // qDebug() << "Vehicle isArmed called, returning:" << m_isArmed;
    return m_isArmed;
}

QString Vehicle::flightMode() const {
    return m_flightMode;
}

QString Vehicle::displayId() const {
    if (m_dataSource == LocalMavlink) {
        return QString::number(m_systemId);
    }
    if (m_dataSource == TeknofestAPI) {
        return QString::number(m_teamId);
    }
    return QString::number(m_vehicleId);
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
    if (qIsNaN(coordinate.latitude()) || qIsNaN(coordinate.longitude())) {
        // qWarning() << "Attempted to set coordinate with NaN values, ignoring.";
        return;
    }
    m_coordinate = coordinate;
    emit coordinateChanged();
}

void Vehicle::setRelativeAltitude(double relativeAltitude) {
    if (m_relativeAltitude == relativeAltitude) {
        return;
    }
    if (qIsNaN(relativeAltitude)) {
        // qWarning() << "Attempted to set relative altitude to NaN, ignoring.";
        return;
    }
    m_relativeAltitude = relativeAltitude;
    emit altitudeChanged(); // Notify that altitude has changed
}

void Vehicle::setAltitude(double altitude) {
    if (m_altitude == altitude) {
        return;
    }
    if (qIsNaN(altitude)) {
        // qWarning() << "Attempted to set altitude to NaN, ignoring.";
        return;
    }
    m_altitude = altitude;
    emit altitudeChanged();
}

void Vehicle::setGroundSpeed(double speed) {
    if (m_groundSpeed == speed) {
        return;
    }
    if (qFuzzyCompare(m_groundSpeed, speed)) {
        // qWarning() << "Attempted to set ground speed to a value that is too close to the current value, ignoring.";
        return;
    }
    m_groundSpeed = speed;
    emit groundSpeedChanged();
}

void Vehicle::setHeading(double heading) {
    if (m_heading == heading) {
        return;
    }
    if (qFuzzyCompare(m_heading, heading)) {
        // qWarning() << "Attempted to set heading to a value that is too close to the current value, ignoring.";
        return;
    }
    m_heading = heading;
    emit headingChanged();
}

void Vehicle::setRoll(double roll) {
    if (m_roll == roll) {
        return;
    }
    if (qFuzzyCompare(m_roll, roll)) {
        // qWarning() << "Attempted to set roll to a value that is too close to the current value, ignoring.";
        return;
    }
    m_roll = roll;
    emit rollChanged();
}

void Vehicle::setPitch(double pitch) {
    if (m_pitch == pitch) {
        return;
    }
    if (qFuzzyCompare(m_pitch, pitch)) {
        // qWarning() << "Attempted to set pitch to a value that is too close to the current value, ignoring.";
        return;
    }
    m_pitch = pitch;
    emit pitchChanged();
}

void Vehicle::setBatteryVoltage(double voltage) {
    if (qFuzzyCompare(m_batteryVoltage, voltage)) {
        return;
    }
    if (qIsNaN(voltage)) {
        // qWarning() << "Attempted to set battery voltage to NaN, ignoring.";
        return;
    }
    m_batteryVoltage = voltage;
    emit batteryVoltageChanged();
}

void Vehicle::setBatteryRemaining(double remaining) {
    if (qFuzzyCompare(m_batteryRemaining, remaining)) {
        return;
    }
    if (qIsNaN(remaining)) {
        // qWarning() << "Attempted to set battery remaining to NaN, ignoring.";
        return;
    }
    m_batteryRemaining = remaining;
    emit batteryRemainingChanged();
}

void Vehicle::setBatteryCurrent(double current) {
    if (qFuzzyCompare(m_batteryCurrent, current)) {
        return;
    }
    if (qIsNaN(current)) {
        // qWarning() << "Attempted to set battery current to NaN, ignoring.";
        return;
    }
    m_batteryCurrent = current;
    emit batteryCurrentChanged();
}

void Vehicle::setOptionalArmed(std::optional<bool> armed) {
    if (!armed.has_value()) {
        return; // Do not set armed state to NULL
    }
    setIsArmed(armed.value());
}

void Vehicle::setIsArmed(bool armed) {
    // qDebug() << "Vehicle setIsArmed called with:" << armed << ", current state:" << m_isArmed;
    if (m_isArmed == armed) {
        return;
    }
    qDebug() << "Vehicle isArmed changed from" << m_isArmed << "to" << armed;
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
