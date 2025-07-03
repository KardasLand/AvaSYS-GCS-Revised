//
// Created by kardasland on 6/27/25.
//

#ifndef APPCONTEXT_H
#define APPCONTEXT_H

#include "../managers/VehicleManager.h"
#include "../managers/MavlinkManager.h"
#include "../managers/TeknofestClient.h"
#include <QObject>

class AppContext : public QObject
{
    Q_OBJECT
    Q_PROPERTY(VehicleManager* vehicleManager READ vehicleManager CONSTANT)
    Q_PROPERTY(MavlinkManager* mavlinkManager READ mavlinkManager CONSTANT)
    Q_PROPERTY(TeknofestClient* teknofestClient READ teknofestClient CONSTANT)

public:
    static AppContext* instance();

    VehicleManager* vehicleManager() const;
    MavlinkManager* mavlinkManager() const;
    TeknofestClient* teknofestClient() const;
    void initializeMavlink();
    void initializeTeknofestServer();

private:
    explicit AppContext(QObject* parent = nullptr);
    AppContext(const AppContext&) = delete;
    AppContext& operator=(const AppContext&) = delete;

    VehicleManager* m_vehicleManager;
    MavlinkManager* m_mavlinkManager;
    TeknofestClient* m_teknofestClient;
};



#endif //APPCONTEXT_H
