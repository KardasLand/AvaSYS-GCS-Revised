//
// Created by kardasland on 6/27/25.
//

#ifndef VEHICLEMANAGER_H
#define VEHICLEMANAGER_H

#include "../core/Vehicle.h"
#include <QObject>
#include <QMap>
#include <QList>

class VehicleManager : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QList<Vehicle*> vehicles READ vehicles NOTIFY vehiclesChanged)
    Q_PROPERTY(Vehicle* selectedVehicle READ selectedVehicle NOTIFY selectedVehicleChanged)

public:
    explicit VehicleManager(QObject *parent = nullptr);

    QList<Vehicle*> vehicles() const;
    Vehicle* selectedVehicle() const;

public slots:
    // Called by MavlinkManager
    void updateMavlinkVehicle(int systemId, const QGeoCoordinate &coord, double alt, double speed, double heading, bool isArmed);

    // Called by TeknofestClient
    void updateTeknofestVehicles(const QJsonArray &vehicleData);

    // Called from QML
    Q_INVOKABLE void selectVehicle(int vehicleId);

    signals:
        void vehiclesChanged();
    void selectedVehicleChanged();

private:
    Vehicle* getOrCreateMavlinkVehicle(int systemId);
    Vehicle* getOrCreateTeknofestVehicle(int teamId);

    QMap<int, Vehicle*> m_vehicles; // Keyed by unique vehicleId
    Vehicle* m_selectedVehicle = nullptr;
    int m_nextVehicleId = 0;
};


#endif //VEHICLEMANAGER_H
