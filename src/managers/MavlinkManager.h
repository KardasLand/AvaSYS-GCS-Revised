#ifndef MAVLINKMANAGER_H
#define MAVLINKMANAGER_H

#include <QObject>
#include <QGeoCoordinate>
#include <QMap>
#include <QPair>
#include <mavlink/common/mavlink.h>
#include <QUdpSocket>
#include <QSerialPort>
#include <QHostAddress>
#include <QTimer>


class MavlinkManager : public QObject
{
    Q_OBJECT
    Q_PROPERTY(bool isConnected READ isConnected NOTIFY isConnectedChanged)
    Q_PROPERTY(QString connectionStatusString READ connectionStatusString NOTIFY isConnectedChanged)
    Q_PROPERTY(QString host READ host WRITE setHost NOTIFY hostChanged)
    Q_PROPERTY(int port READ port WRITE setPort NOTIFY portChanged)
    Q_PROPERTY(QString serialPort READ serialPort WRITE setSerialPort NOTIFY serialPortChanged)
    Q_PROPERTY(COMMUNICATION_TYPE communicationType READ communicationType WRITE setCommunicationType NOTIFY communicationTypeChanged)
    Q_PROPERTY(int baudrate READ baudrate WRITE setBaudrate NOTIFY baudrateChanged)

public:
    explicit MavlinkManager(QObject *parent = nullptr);

    enum COMMUNICATION_TYPE {
        UDP,
        SERIAL
    };
    Q_ENUM(COMMUNICATION_TYPE)

    bool isConnected() const;
    QString connectionStatusString() const;

    QString serialPort() const { return m_serialPortName; }
    Q_INVOKABLE void setSerialPort(const QString portName) {
        if (portName.isEmpty()) {
            qWarning() << "Serial port name cannot be empty.";
            return;
        }
        if (m_serialPortName == portName) return; // No change
        m_serialPortName = portName;
        emit serialPortChanged();
    }

    Q_INVOKABLE void setMavlinkUdpSettings(const QString &host, int port) {
        setHost(host);
        setPort(port);
    }

    COMMUNICATION_TYPE communicationType() const {
        return m_communicationType;
    }



    int baudrate() const { return m_baudrate; }



    Q_INVOKABLE void setBaudrate(int baudrate) {
        if (baudrate < 0) {
            qWarning() << "Invalid baudrate:" << baudrate;
            return;
        }
        if (m_baudrate == baudrate) return; // No change
        m_baudrate = baudrate;
        emit baudrateChanged();
    }

    Q_INVOKABLE void set_comm_type_qml(int type) {
        auto commType = static_cast<COMMUNICATION_TYPE>(type);
        setCommunicationType(commType);
    }

    void setCommunicationType(COMMUNICATION_TYPE type) {
        if (m_communicationType == type) return; // No change
        qDebug() << "Setting communication type to" << (type == UDP ? "UDP" : "SERIAL");
        m_communicationType = type;
        emit communicationTypeChanged();
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
    void sendHeartbeat(QHostAddress &address, quint16 port);
public slots:
    Q_INVOKABLE void connectUdp(const QString& ip, quint16 port);

    Q_INVOKABLE void disconnect();
    Q_INVOKABLE void sendArmCommand(int systemId, bool arm, bool force = false);
    Q_INVOKABLE void sendTakeoffCommand(int systemId, double altitude);
    Q_INVOKABLE void sendLandCommand(int systemId);

signals:
    void isConnectedChanged();
    void hostChanged();
    void portChanged();
    void baudrateChanged();
    void serialPortChanged();
    void communicationTypeChanged();
    void connectionFailed(const QString& error);

    // The single, powerful signal to update the VehicleManager
    void mavlinkVehicleUpdated(int systemId, const QGeoCoordinate &coord, double alt, double relativeAltitude,
                               double speed, double heading, double roll, double pitch,
                               double batteryRemaining, double batteryVoltage, double batteryCurrent, std::optional<bool> isArmed, const QString& flightMode);


private slots:
    void onUdpReadyRead();

    void onSerialReadyRead();

private:
    void parseMavlinkData(const QByteArray& data, const QHostAddress& senderAddress, quint16 senderPort);
    void sendMavlinkCommandLong(int systemId, uint16_t command, float p1 = 0, float p2 = 0, float p3 = 0, float p4 = 0, float p5 = 0, float p6 = 0, float p7 = 0);

    QUdpSocket* m_udpSocket;
    QSerialPort* m_serialPort;
    int m_baudrate;
    COMMUNICATION_TYPE m_communicationType = UDP; // Default to UDP
    bool m_isConnected = false;
    QString m_connectionStatusString;
    QString m_serialPortName; // Name of the serial port if using SERIAL communication
    QString m_host;
    int m_port = 14550; // Default MAVLink port
    int m_senderPort; // Default sender port for GCS
    QTimer* m_heartbeatTimer = nullptr;

    // Maps a vehicle's system ID to its network endpoint (IP and port).
    // This is crucial for sending commands back to the correct vehicle.
    QMap<int, QPair<QHostAddress, quint16>> m_vehicleEndpoints;
};

#endif
