#include <QGuiApplication>
#include <QQmlApplicationEngine>

#include "core/AppContext.h"
#include "managers/MavlinkManager.h"
#include "managers/TeknofestClient.h"
#include "managers/VehicleManager.h"

int main(int argc, char *argv[])
{
    const QGuiApplication app(argc, argv);

    QQmlApplicationEngine engine;

    qmlRegisterSingletonInstance<AppContext>("App.Context", 1, 0, "AppContext", AppContext::instance());
    qmlRegisterUncreatableType<VehicleManager>("App.Managers", 1, 0, "VehicleManager", "Cannot create VehicleManager in QML");
    qmlRegisterUncreatableType<MavlinkManager>("App.Managers", 1, 0, "MavlinkManager", "Cannot create MavlinkManager in QML");
    qmlRegisterUncreatableType<TeknofestClient>("App.Managers", 1, 0, "TeknofestClient", "Cannot create TeknofestClient in QML");
    qmlRegisterType<Vehicle>("App.Models", 1, 0, "Vehicle");

    QObject::connect(
        &engine,
        &QQmlApplicationEngine::objectCreationFailed,
        &app,
        []() { QCoreApplication::exit(-1); },
        Qt::QueuedConnection);
    engine.loadFromModule("AvaSYS2", "Main");

    return QGuiApplication::exec();
}
