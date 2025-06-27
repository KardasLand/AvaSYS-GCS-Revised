//
// Created by kardasland on 6/27/25.
//

#ifndef MAVLINKMANAGER_H
#define MAVLINKMANAGER_H

#include <QObject>
#include <QSerialPort>
#include <QUdpSocket>
#include <QGeoCoordinate>

class MavlinkManager : public QObject
{
    Q_OBJECT
    Q_PROPERTY(bool isConnected READ isConnected NOTIFY isConnectedChanged)

public:
    explicit MavlinkManager(QObject *parent = nullptr);

    bool isConnected() const;

public slots:
    Q_INVOKABLE void connectUdp(const QString& ip, quint16 port);
    Q_INVOKABLE void connectSerial(const QString& portName, qint32 baudRate);
    Q_INVOKABLE void disconnect();
    Q_INVOKABLE void sendArmCommand(int systemId, bool arm);

    signals:
        void isConnectedChanged();
    // This signal is connected to the VehicleManager
    void mavlinkVehicleDataReady(int systemId, const QGeoCoordinate &coord, double alt, double speed, double heading, bool isArmed);

private slots:
    void onUdpReadyRead();
    void onSerialReadyRead();
    void parseMavlinkData(const QByteArray& data);

private:
    QUdpSocket* m_udpSocket = nullptr;
    QSerialPort* m_serialPort = nullptr;
    bool m_isConnected = false;
    QHostAddress m_gcsAddress;
    quint16 m_gcsPort = 0;
};



#endif //MAVLINKMANAGER_H
