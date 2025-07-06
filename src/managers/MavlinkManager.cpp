#include "MavlinkManager.h"
#include <QUdpSocket>
#include <QHostAddress>
#include <QDebug>
#include <cmath>

#include "src/core/AppContext.h"

// Helper to convert radians to degrees
static double radToDeg(double rad) {
    return rad * 180.0 / M_PI;
}

MavlinkManager::MavlinkManager(QObject *parent)
    : QObject(parent),
      m_udpSocket(new QUdpSocket(this)),
      m_isConnected(false),
      m_connectionStatusString("Disconnected")
{
    // Register custom types for signal/slot connections if needed elsewhere
    qRegisterMetaType<QGeoCoordinate>("QGeoCoordinate");

    // Connect the socket's readyRead signal to our handler
    connect(m_udpSocket, &QUdpSocket::readyRead, this, &MavlinkManager::onUdpReadyRead);
}

bool MavlinkManager::isConnected() const {
    return m_isConnected;
}

QString MavlinkManager::connectionStatusString() const {
    return m_connectionStatusString;
}

void MavlinkManager::connectUdp(const QString& ip, quint16 port) {
    disconnect(); // Ensure we are in a clean state

    // For a GCS, we bind to a local port to listen for incoming data from any vehicle.
    if (!m_udpSocket->bind(QHostAddress::AnyIPv4, port, QUdpSocket::ShareAddress | QUdpSocket::ReuseAddressHint)) {
        QString error = QString("UDP Bind Failed: %1").arg(m_udpSocket->errorString());
        qWarning() << error;
        emit connectionFailed(error);
        return;
    }

    m_isConnected = true;
    m_connectionStatusString = QString("Listening on UDP Port %1").arg(port);
    qInfo() << m_connectionStatusString;

    emit isConnectedChanged();
}

void MavlinkManager::connectFromSettings() {
    disconnect();
    // For a GCS, we bind to a local port to listen for incoming data from any vehicle.
    if (!m_udpSocket->bind(QHostAddress::AnyIPv4, m_port, QUdpSocket::ShareAddress | QUdpSocket::ReuseAddressHint)) {
        QString error = QString("UDP Bind Failed: %1").arg(m_udpSocket->errorString());
        qWarning() << error;
        emit connectionFailed(error);
        return;
    }
    // send_heartbeat();

    m_isConnected = true;
    m_connectionStatusString = QString("Listening on UDP Port %1").arg(m_port);
    qInfo() << m_connectionStatusString;
    // sendHeartbeat();
    emit isConnectedChanged();
}

void MavlinkManager::disconnect() {
    if (m_isConnected) {
        m_udpSocket->close();
        m_isConnected = false;
        m_connectionStatusString = "Disconnected";
        m_vehicleEndpoints.clear();
        qInfo() << "UDP socket disconnected.";
        emit isConnectedChanged();
    }
}

void MavlinkManager::onUdpReadyRead() {
    while (m_udpSocket->hasPendingDatagrams()) {
        QByteArray datagram;
        datagram.resize(m_udpSocket->pendingDatagramSize());
        QHostAddress senderAddress;
        quint16 senderPort;

        m_udpSocket->readDatagram(datagram.data(), datagram.size(), &senderAddress, &senderPort);

        parseMavlinkData(datagram, senderAddress, senderPort);

        sendHeartbeat(senderAddress, senderPort);

    }
}

void MavlinkManager::sendHeartbeat(QHostAddress &address, quint16 port) {
    mavlink_message_t message;
    mavlink_heartbeat_t heartbeat;
    heartbeat.type = MAV_TYPE_GCS;
    heartbeat.autopilot = MAV_AUTOPILOT_INVALID;
    heartbeat.base_mode = 0;
    heartbeat.custom_mode = 0;
    heartbeat.system_status = 0;
    Vehicle* vehicle = AppContext::instance()->vehicleManager()->getMainVehicle();
    mavlink_msg_heartbeat_encode(vehicle->systemId(), 200, &message, &heartbeat);
    uint8_t buffer[MAVLINK_MAX_PACKET_LEN];
    const int len = mavlink_msg_to_send_buffer(buffer, &message);
    // qDebug() << "Sending MAVLink heartbeat from GCS with system ID" << vehicle->systemId();
    // qDebug() << "Sending MAVLink heartbeat to" << address.toString() << ":" << port;
    m_udpSocket->writeDatagram(reinterpret_cast<const char*>(buffer), len, address, port);
}

void MavlinkManager::parseMavlinkData(const QByteArray& data, const QHostAddress& senderAddress, quint16 senderPort) {
    mavlink_message_t msg;
    mavlink_status_t status;

    for (int i = 0; i < data.size(); ++i) {
        if (mavlink_parse_char(MAVLINK_COMM_0, static_cast<uint8_t>(data[i]), &msg, &status)) {
            // A full MAVLink message has been received.
            // Remember this vehicle's endpoint so we can send commands back to it.
            m_vehicleEndpoints[msg.sysid] = {senderAddress, senderPort};

            // Temporary variables to hold data before emitting the signal
            QGeoCoordinate coord;
            double altitude = NAN;
            double relativeAltitude = NAN; // Relative altitude if needed
            double groundSpeed = NAN;
            double heading = NAN;
            double roll = NAN;
            double pitch = NAN;
            double batteryRemaining = NAN;
            double batteryVoltage = NAN;
            double batteryCurrent = NAN;
            std::optional<bool> isArmed = std::nullopt;
            QString flightMode = "Unknown";
            bool shouldUpdate = false;

            switch (msg.msgid) {
                case MAVLINK_MSG_ID_HEARTBEAT: {
                    mavlink_heartbeat_t heartbeat;
                    mavlink_msg_heartbeat_decode(&msg, &heartbeat);
                    if (heartbeat.type == MAV_TYPE_GCS) break; // Ignore other GCS heartbeats

                    // qDebug() << "Received Heartbeat: base_mode:" << heartbeat.base_mode
                    // << "custom_mode:" << heartbeat.custom_mode
                    // << "type:" << heartbeat.type
                    // << "autopilot:" << heartbeat.autopilot;;
                    isArmed = (heartbeat.base_mode & MAV_MODE_FLAG_SAFETY_ARMED) != 0;
                    // qDebug() << "Is armed:" << isArmed;
                    // isArmed = (heartbeat.base_mode & MAV_MODE_FLAG_SAFETY_ARMED);
                    // qDebug() << "Heartbeat received from vehicle" << msg.sysid
                    //          << "Type:" << heartbeat.type
                    //          << "Armed:" << isArmed;
                    // qDebug() << "Heartbeat from vehicle" << msg.sysid
                    //          << "Type:" << heartbeat.type
                    //          << "Armed:" << (isArmed ? "Yes" : "No")
                    //          << "Base Mode:" << heartbeat.base_mode;
                    flightMode = QString("Mode %1").arg(heartbeat.custom_mode);
                    if (heartbeat.custom_mode == 0) {
                        flightMode = "Unknown";
                    } else if (heartbeat.custom_mode == 1) {
                        flightMode = "Manual";
                    } else if (heartbeat.custom_mode == 2) {
                        flightMode = "Stabilize";
                    } else if (heartbeat.custom_mode == 3) {
                        flightMode = "Guided";
                    } else if (heartbeat.custom_mode == 4) {
                        flightMode = "Auto";
                    }
                    shouldUpdate = true; // Heartbeat is a good reason to update state
                    break;
                }

                case MAVLINK_MSG_ID_GLOBAL_POSITION_INT: {
                    mavlink_global_position_int_t pos;
                    mavlink_msg_global_position_int_decode(&msg, &pos);

                    coord.setLatitude(pos.lat / 1e7);
                    coord.setLongitude(pos.lon / 1e7);
                    relativeAltitude = pos.relative_alt / 1000; // Relative altitude in meters
                    altitude = pos.alt / 1000; // Relative altitude in meters
                    heading = pos.hdg / 100; // Heading in degrees
                    // qDebug() << "Global Position from vehicle" << msg.sysid
                    //          << "Lat:" << coord.latitude()
                    //          << "Lon:" << coord.longitude()
                    //          << "Alt:" << altitude
                    //          << "Heading:" << heading;
                    shouldUpdate = true;
                    break;
                }

                case MAVLINK_MSG_ID_VFR_HUD: {
                    mavlink_vfr_hud_t vfr;
                    mavlink_msg_vfr_hud_decode(&msg, &vfr);
                    groundSpeed = vfr.groundspeed;
                    // Note: altitude and heading are also here, could be a fallback.
                    shouldUpdate = true;
                    break;
                }

                case MAVLINK_MSG_ID_ATTITUDE: {
                    mavlink_attitude_t attitude;
                    mavlink_msg_attitude_decode(&msg, &attitude);
                    roll = radToDeg(attitude.roll);
                    pitch = radToDeg(attitude.pitch);
                    // Yaw from ATTITUDE is often used for the horizon, while heading from GPS is used for the map.
                    // Here we will use the GPS heading for consistency.
                    shouldUpdate = true;
                    break;
                }

                case MAVLINK_MSG_ID_SYS_STATUS: {
                    mavlink_sys_status_t sysStatus;
                    mavlink_msg_sys_status_decode(&msg, &sysStatus);

                    // qDebug() << "Raw sys_status from vehicle" << msg.sysid
                    //          << "Battery Remaining:" << sysStatus.battery_remaining
                    //          << "Voltage (mV):" << sysStatus.voltage_battery
                    //          << "Current (mA):" << sysStatus.current_battery;
                    //
                    //
                    // qDebug() << "Vehicle" << msg.sysid
                    //          << "Battery Remaining:" << sysStatus.battery_remaining
                    //          << "Voltage (V):" << sysStatus.voltage_battery / 1000.0 // Convert mV to V
                    //          << "Current (A):" << sysStatus.current_battery / 1000.0; // Convert mA to A

                    batteryRemaining = sysStatus.battery_remaining;
                    batteryVoltage = sysStatus.voltage_battery / 1000.0; // Convert mV to V
                    batteryCurrent = sysStatus.current_battery / 1000.0; // Convert mA to A
                    shouldUpdate = true;
                    break;
                }

                default:
                    break;
            }

            // qDebug() << "is armed: " << isArmed;

            if (shouldUpdate) {
                // qDebug() << "Updating vehicle" << msg.sysid
                //          << "at" << coord
                //          << "Alt:" << altitude
                //          << "Speed:" << groundSpeed
                //          << "Heading:" << heading
                //          << "Roll:" << roll
                //          << "Pitch:" << pitch
                //          << "Battery:" << battery
                //          << "Armed:" << isArmed
                //          << "Flight Mode:" << flightMode;
                // Emit one signal with all the new data for this vehicle
                emit mavlinkVehicleUpdated(msg.sysid, coord, altitude, relativeAltitude, groundSpeed,
                                           heading, roll, pitch, batteryRemaining, batteryVoltage, batteryCurrent, isArmed, flightMode);
            }
        }
    }
}

void MavlinkManager::sendMavlinkCommandLong(int systemId, uint16_t command, float p1, float p2, float p3, float p4, float p5, float p6, float p7) {
    if (!m_isConnected || !m_vehicleEndpoints.contains(systemId)) {
        qWarning() << "Cannot send command: Not connected or vehicle endpoint unknown for system ID" << systemId;
        return;
    }

    mavlink_message_t msg;
    uint8_t buffer[MAVLINK_MAX_PACKET_LEN];
    // Our GCS system ID is 255, component is MAV_COMP_ID_MISSIONPLANNER
    uint8_t ourSystemId = 255;
    // 190 previously
    uint8_t ourComponentId = 200;

    mavlink_msg_command_long_pack(
        ourSystemId, ourComponentId, &msg,
        systemId, MAV_COMP_ID_AUTOPILOT1, // Target system and component
        command, 0, // Command and confirmation
        p1, p2, p3, p4, p5, p6, p7
    );
    qDebug() << "Sending MAVLink command" << command << "to vehicle" << systemId;

    uint16_t len = mavlink_msg_to_send_buffer(buffer, &msg);
    auto endpoint = m_vehicleEndpoints.value(systemId);
    m_udpSocket->writeDatagram(reinterpret_cast<const char*>(buffer), len, endpoint.first, endpoint.second);
}

void MavlinkManager::sendArmCommand(int systemId, bool arm, bool force) {
    qInfo() << (arm ? "ARMING" : "DISARMING") << "vehicle" << systemId << (force ? "with force." : ".");
    float p1 = arm ? 1 : 0;
    float p2 = force ? 21196 : 0; // Magic number for force arm/disarm
    // sendMavlinkCommandLong(systemId, MAV_CMD_COMPONENT_ARM_DISARM, p1, p2);
    mavlink_message_t msg;
    mavlink_msg_command_long_pack(255, 200, &msg,
                                  systemId, 1, // Target System and Component
                                  MAV_CMD_COMPONENT_ARM_DISARM, 0, p1,p2, 0, 0, 0, 0, 0);
    uint8_t buf[MAVLINK_MAX_PACKET_LEN];
    uint16_t len = mavlink_msg_to_send_buffer(buf, &msg);
    auto endpoint = m_vehicleEndpoints.value(systemId);
    if (endpoint.first.isNull() || endpoint.second == 0) {
        qWarning() << "Cannot send command: No endpoint for system ID" << systemId;
        return;
    }
    m_udpSocket->writeDatagram(reinterpret_cast<const char*>(buf), len, endpoint.first, endpoint.second);
}

void MavlinkManager::sendTakeoffCommand(int systemId, double altitude) {
    qInfo() << "Sending TAKEOFF command to vehicle" << systemId << "to altitude" << altitude << "m.";
    // Params for MAV_CMD_NAV_TAKEOFF:
    // p4: Yaw (NaN for current), p5: Lat, p6: Lon, p7: Alt
    sendMavlinkCommandLong(systemId, MAV_CMD_NAV_TAKEOFF, 0, 0, 0, NAN, 0, 0, altitude);
}

void MavlinkManager::sendLandCommand(int systemId) {
    qInfo() << "Sending LAND command to vehicle" << systemId;
    sendMavlinkCommandLong(systemId, MAV_CMD_NAV_LAND);
}