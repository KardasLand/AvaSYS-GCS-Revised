//
// Created by kardasland on 6/27/25.
//
#ifndef VEHICLE_H
#define VEHICLE_H
#include <qobject.h>
#include <qtmetamacros.h>
#include <QGeoCoordinate>
#include <QQmlEngine> // <-- Include for QML_ELEMENT

QML_ELEMENT
class Vehicle : public QObject{
    Q_OBJECT
    Q_PROPERTY(int vehicleId READ vehicleId CONSTANT)
    Q_PROPERTY(DataSource dataSource READ dataSource CONSTANT)
    Q_PROPERTY(int systemId READ systemId NOTIFY systemIdChanged)
    Q_PROPERTY(int teamId READ teamId NOTIFY teamIdChanged)
    Q_PROPERTY(bool isSelected READ isSelected NOTIFY isSelectedChanged)

    Q_PROPERTY(QGeoCoordinate coordinate READ coordinate NOTIFY coordinateChanged)
    Q_PROPERTY(double altitude READ altitude NOTIFY altitudeChanged)
    Q_PROPERTY(double relativeAltitude READ altitude NOTIFY altitudeChanged) // Relative altitude if needed
    Q_PROPERTY(double groundSpeed READ groundSpeed NOTIFY groundSpeedChanged)
    Q_PROPERTY(double heading READ heading NOTIFY headingChanged)
    Q_PROPERTY(double roll READ roll NOTIFY rollChanged)
    Q_PROPERTY(double pitch READ pitch NOTIFY pitchChanged)
    Q_PROPERTY(double batteryVoltage READ batteryVoltage NOTIFY batteryVoltageChanged)
    Q_PROPERTY(double batteryRemaining READ batteryRemaining NOTIFY batteryRemainingChanged)
    Q_PROPERTY(double batteryCurrent READ batteryCurrent NOTIFY batteryCurrentChanged)
    Q_PROPERTY(bool isArmed READ isArmed NOTIFY isArmedChanged)
    Q_PROPERTY(QString flightMode READ flightMode NOTIFY flightModeChanged)
    Q_PROPERTY(bool isFlying READ isFlying CONSTANT) // Derived property, not stored

    Q_PROPERTY(QString displayId READ displayId NOTIFY displayIdChanged)
    Q_PROPERTY(QString altitudeString READ altitudeString NOTIFY altitudeChanged)
    Q_PROPERTY(QString groundSpeedString READ groundSpeedString NOTIFY groundSpeedChanged)

public:
    enum DataSource {
        SourceUnknown,
        LocalMavlink,
        TeknofestAPI
    };
    Q_ENUM(DataSource)
    explicit Vehicle(int vehicleId, DataSource dataSource, QObject *parent = nullptr);
    int vehicleId() const;
    DataSource dataSource() const;
    int systemId() const;
    int teamId() const;
    Q_INVOKABLE bool isSelected() const;
    QGeoCoordinate coordinate() const;
    double altitude() const;
    double relativeAltitude() const; // Relative altitude if needed

    Q_INVOKABLE double groundSpeed() const;
    double heading() const;
    double roll() const;
    double pitch() const;

    double batteryVoltage() const;
    double batteryRemaining() const;
    double batteryCurrent() const;


    bool isArmed() const;
    QString flightMode() const;
    QString displayId() const;
    QString altitudeString() const;
    QString groundSpeedString() const;

    bool isFlying() const {
        return m_isArmed && m_relativeAltitude > 1.5 && m_groundSpeed > 0.5;
    }

public slots:
    void setSystemId(int id);
    void setTeamId(int id);
    void setIsSelected(bool selected);
    void setCoordinate(const QGeoCoordinate &coordinate);
    void setRelativeAltitude(double relativeAltitude); // Relative altitude if needed
    void setAltitude(double altitude);
    void setGroundSpeed(double speed);
    void setHeading(double heading);
    void setRoll(double roll);
    void setPitch(double pitch);
    void setBatteryVoltage(double voltage);
    void setBatteryRemaining(double remaining);
    void setBatteryCurrent(double current);

    void setOptionalArmed(std::optional<bool> armed);

    void setIsArmed(bool armed);
    void setFlightMode(const QString& mode);
signals:
    void systemIdChanged();
    void teamIdChanged();
    void isSelectedChanged();
    void coordinateChanged();
    void altitudeChanged();
    void relativeAltitudeChanged(); // Relative altitude if needed
    void groundSpeedChanged();
    void headingChanged();
    void rollChanged();
    void pitchChanged();
    void batteryVoltageChanged();
    void batteryRemainingChanged();
    void batteryCurrentChanged();
    void isArmedChanged();
    void flightModeChanged();
    void displayIdChanged();
private:
    const int m_vehicleId;
    const DataSource m_dataSource;

    int m_systemId = -1;
    int m_teamId = -1;
    bool m_isSelected = false;

    QGeoCoordinate m_coordinate;
    double m_altitude = 0.0;
    double m_relativeAltitude = 0.0; // Relative altitude if needed
    double m_groundSpeed = 0.0;
    double m_heading = 0.0;
    double m_roll = 0.0;
    double m_pitch = 0.0;
    double m_batteryVoltage = 0.0;
    double m_batteryRemaining = 0.0; // Percentage from 0 to 100
    double m_batteryCurrent = 0.0; // Current in Amperes, if available
    bool m_isArmed = false;
    QString m_flightMode = "Unknown";
};

#endif // VEHICLE_H