//
// Created by kardasland on 6/27/25.
//

#include "MavlinkManager.h"

MavlinkManager::MavlinkManager(QObject *parent) : QObject(parent), m_udpSocket(nullptr), m_serialPort(nullptr), m_isConnected(false) {
    qRegisterMetaType<QGeoCoordinate>("QGeoCoordinate");
    qRegisterMetaType<MavlinkManager>("MavlinkManager");
    qRegisterMetaType<int32_t>("int32_t");
}

bool MavlinkManager::isConnected() const {
    return m_isConnected;
}

void MavlinkManager::connectUdp(const QString &ip, quint16 port) {

}

void MavlinkManager::connectSerial(const QString &portName, qint32 baudRate) {
}

void MavlinkManager::disconnect() {
}

void MavlinkManager::sendArmCommand(int systemId, bool arm) {
}

void MavlinkManager::onUdpReadyRead() {
}

void MavlinkManager::onSerialReadyRead() {
}

void MavlinkManager::parseMavlinkData(const QByteArray &data) {
}
