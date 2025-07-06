#pragma once

#include "../core/Vehicle.h"
#include <QObject>
#include <QList>
#include <QMap>
#include <QTimer>
#include <QQmlListProperty>

class QJsonArray;

class VehicleManager : public QObject
{
    Q_OBJECT
    
    Q_PROPERTY(QQmlListProperty<Vehicle> vehicles READ vehicles NOTIFY vehiclesChanged)

    Q_PROPERTY(Vehicle* mainVehicle READ getMainVehicle NOTIFY mainVehicleChanged)

    Q_PROPERTY(Vehicle* selectedVehicle READ selectedVehicle NOTIFY selectedVehicleChanged)

public:
    explicit VehicleManager(QObject *parent = nullptr);

    // --- QML-accessible properties and methods ---

    // Getter for the 'vehicles' property
    QQmlListProperty<Vehicle> vehicles();
    
    // Getter for the 'selectedVehicle' property
    Vehicle* selectedVehicle() const;


    Q_INVOKABLE Vehicle* getMainVehicle() const;

    // Called from QML to change the selected vehicle, using its unique internal ID.
    Q_INVOKABLE void selectVehicle(int vehicleId);

    void clearTeknofestVehicles();

public slots:
    // Called by MavlinkManager to update vehicle data
    void updateMavlinkVehicle(int systemId, const QGeoCoordinate &coord, double alt, double relativeAltitude,
                              double speed, double heading, double roll, double pitch,
                              double batteryRemaining, double batteryVoltage, double batteryCurrent, std::optional<bool> isArmed, const QString& flightMode);

    // Called by TeknofestClient to update vehicle data
    void updateTeknofestVehicles(const QJsonArray &vehicleData);
    void startTransmittingTelemetry(int teamid);
    void stopTransmittingTelemetry();
signals:
    // Emitted whenever the list of vehicles changes (add/remove).
    void vehiclesChanged();

    void mainVehicleChanged();

    void transmitTelemetryRequest(QJsonObject &vehicleData);
    void hssCoordinateRequest();

    // Emitted whenever the selected vehicle changes.
    void selectedVehicleChanged();

private:
    static void append_vehicle(QQmlListProperty<Vehicle>* list, Vehicle* vehicle);

    // These static functions are required by QQmlListProperty to access our list.
    static qsizetype vehicle_count(QQmlListProperty<Vehicle>* list);
    static Vehicle* vehicle_at(QQmlListProperty<Vehicle>* list, qsizetype index);
    
    // Private helper methods for finding or creating vehicles
    Vehicle* getOrCreateMavlinkVehicle(int systemId);
    Vehicle* getOrCreateTeknofestVehicle(int teamId);

    // --- Data Members ---

    // The single source of truth for all vehicles, mapped by their unique internal ID.
    // This is used for fast lookups.
    QMap<int, Vehicle*> m_vehicleMap;
    
    // The ordered list of vehicles exposed to QML. This provides a stable order.
    QList<Vehicle*> m_vehicleList;
    
    // Pointer to the currently selected vehicle.
    Vehicle* m_selectedVehicle = nullptr;

    QTimer* m_telemetryTimer;
    
    // Counter to generate new unique internal IDs.
    int m_nextVehicleId = 0;
    
    // Lookup maps to find our internal vehicle ID from a source-specific ID.
    QMap<int, int> m_mavlinkIdToVehicleId;   // MAVLink System ID -> Internal Vehicle ID
    QMap<int, int> m_teknofestIdToVehicleId; // Teknofest Team ID -> Internal Vehicle ID
};