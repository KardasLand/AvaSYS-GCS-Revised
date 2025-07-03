#ifndef MAVLINKMANAGER_H
#define MAVLINKMANAGER_H

#include <QObject>
#include <QGeoCoordinate>
#include <QMap>
#include <QPair>
#include <mavlink/common/mavlink.h>
#include <QUdpSocket>
#include <QHostAddress>


class MavlinkManager : public QObject
{
    Q_OBJECT
    Q_PROPERTY(bool isConnected READ isConnected NOTIFY isConnectedChanged)
    Q_PROPERTY(QString connectionStatusString READ connectionStatusString NOTIFY isConnectedChanged)

public:
    explicit MavlinkManager(QObject *parent = nullptr);

    bool isConnected() const;
    QString connectionStatusString() const;

public slots:
    Q_INVOKABLE void connectUdp(const QString& ip, quint16 port);
    Q_INVOKABLE void disconnect();
    Q_INVOKABLE void sendArmCommand(int systemId, bool arm, bool force = false);
    Q_INVOKABLE void sendTakeoffCommand(int systemId, double altitude);
    Q_INVOKABLE void sendReturnToLaunchCommand(int systemId);

signals:
    void isConnectedChanged();
    void connectionFailed(const QString& error);

    // The single, powerful signal to update the VehicleManager
    void mavlinkVehicleUpdated(int systemId, const QGeoCoordinate &coord, double alt,
                               double speed, double heading, double roll, double pitch,
                               double batteryRemaining, double batteryVoltage, double batteryCurrent, bool isArmed, const QString& flightMode);

private slots:
    void onUdpReadyRead();

private:
    void parseMavlinkData(const QByteArray& data, const QHostAddress& senderAddress, quint16 senderPort);
    void sendMavlinkCommandLong(int systemId, uint16_t command, float p1 = 0, float p2 = 0, float p3 = 0, float p4 = 0, float p5 = 0, float p6 = 0, float p7 = 0);

    QUdpSocket* m_udpSocket;
    bool m_isConnected = false;
    QString m_connectionStatusString;

    // Maps a vehicle's system ID to its network endpoint (IP and port).
    // This is crucial for sending commands back to the correct vehicle.
    QMap<int, QPair<QHostAddress, quint16>> m_vehicleEndpoints;
};

#endif
