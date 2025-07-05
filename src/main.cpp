#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QFileInfo>
#include <QDir>
#include <QQmlContext>

#include "core/AppContext.h"
#include "horizon/linearindicator.h"
#include "managers/MavlinkManager.h"
#include "managers/TeknofestClient.h"
#include "managers/VehicleManager.h"

int main(int argc, char *argv[])
{
    const QGuiApplication app(argc, argv);

    QQmlApplicationEngine engine;
    engine.rootContext()->setContextProperty("THUNDERFOREST_API_KEY", qgetenv("THUNDERFOREST_API_KEY"));
    engine.rootContext()->setContextProperty("TEKNOFEST_API_URL", qgetenv("TEKNOFEST_API_URL"));

    AppContext* appContext = AppContext::instance();

    qmlRegisterSingletonInstance<AppContext>("App.Context", 1, 0, "AppContext", appContext);
    qmlRegisterUncreatableType<VehicleManager>("App.Managers", 1, 0, "VehicleManager", "Cannot create VehicleManager in QML");
    qmlRegisterUncreatableType<MavlinkManager>("App.Managers", 1, 0, "MavlinkManager", "Cannot create MavlinkManager in QML");
    qmlRegisterUncreatableType<TeknofestClient>("App.Managers", 1, 0, "TeknofestClient", "Cannot create TeknofestClient in QML");
    qmlRegisterType<LinearIndicator>("io.smth", 1, 0, "LinearIndicator");
    qmlRegisterType<ColorSegment>("io.smth", 1, 0, "ColorSegment");

    QObject::connect(
        &engine,
        &QQmlApplicationEngine::objectCreationFailed,
        &app,
        []() { QCoreApplication::exit(-1); },
        Qt::QueuedConnection);
    engine.loadFromModule("AvaSYS2", "Main");
    appContext->autoConnectMavlink();
    appContext->initializeTeknofestServer();


    return QGuiApplication::exec();
}
