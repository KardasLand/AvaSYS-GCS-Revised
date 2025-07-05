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
    Q_PROPERTY(QString host READ host WRITE setHost NOTIFY hostChanged)
    Q_PROPERTY(int port READ port WRITE setPort NOTIFY portChanged)
public:
    explicit MavlinkManager(QObject *parent = nullptr);

    bool isConnected() const;
    QString connectionStatusString() const;

    Q_INVOKABLE void setMavlinkSettings(const QString &host, int port) {
        setHost(host);
        setPort(port);
    }

    QString host() const {return m_host;}
    void setHost(const QString &m_host) {
        if (m_host.isEmpty()) {
            qWarning() << "Host cannot be empty.";
            return;
        }
        if (m_host == this->m_host) return; // No change
        this->m_host = m_host;
        emit hostChanged();
    }

    int port() const {return m_port;}
    void setPort(int port) {
        if (port < 0 || port > 65535) {
            qWarning() << "Invalid port number:" << port;
            return;
        }
        if (m_port == port) return; // No change
        m_port = port;
        emit portChanged();

    }
    Q_INVOKABLE void connectFromSettings();
public slots:
    Q_INVOKABLE void connectUdp(const QString& ip, quint16 port);

    Q_INVOKABLE void disconnect();
    Q_INVOKABLE void sendArmCommand(int systemId, bool arm, bool force = false);
    Q_INVOKABLE void sendTakeoffCommand(int systemId, double altitude);
    Q_INVOKABLE void sendReturnToLaunchCommand(int systemId);

signals:
    void isConnectedChanged();
    void hostChanged();
    void portChanged();
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
    QString m_host;
    int m_port = 14550; // Default MAVLink port

    // Maps a vehicle's system ID to its network endpoint (IP and port).
    // This is crucial for sending commands back to the correct vehicle.
    QMap<int, QPair<QHostAddress, quint16>> m_vehicleEndpoints;
};

#endif
