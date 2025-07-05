cmake_minimum_required(VERSION 3.16)

project(AvaSYS2 VERSION 0.1 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)
set(QT_QML_GENERATE_QMLLS_INI ON)

find_package(Qt6 REQUIRED COMPONENTS Qml Quick Network SerialPort Positioning Location Gui)
find_package(PkgConfig REQUIRED)
pkg_check_modules(GStreamer REQUIRED gstreamer-1.0 gstreamer-video-1.0)
pkg_check_modules(GST_APP REQUIRED gstreamer-app-1.0)
pkg_check_modules(GST_VIDEO REQUIRED gstreamer-video-1.0)
find_package(MAVLink REQUIRED)
find_package(OpenSSL REQUIRED)
find_package(OpenCV REQUIRED)

qt_standard_project_setup(REQUIRES 6.8)

qt_add_executable(appAvaSYS2
        src/main.cpp
        src/core/Vehicle.cpp
        src/core/Vehicle.h
        src/managers/VehicleManager.cpp
        src/managers/VehicleManager.h
        src/managers/MavlinkManager.cpp
        src/managers/MavlinkManager.h
        src/managers/TeknofestClient.cpp
        src/managers/TeknofestClient.h
        src/core/AppContext.cpp
        src/core/AppContext.h
        src/network/HttpClient.cpp
        src/network/HttpClient.h
        src/core/VideoItem.cpp
        src/core/VideoItem.h
        src/horizon/linearindicator.cpp
        src/horizon/linearindicator.h
        src/horizon/painterhelpers.cpp
        src/horizon/painterhelpers.h
)

qt_add_qml_module(appAvaSYS2
    URI AvaSYS2
    VERSION 1.0
    QML_FILES
        ui/Main.qml
        ui/panels/MainPanel.qml
        ui/panels/BottomPanel.qml
        ui/components/StyledGauge.qml
        ui/components/CompassGauge.qml
        ui/settings/SettingsWindow.qml
        ui/horizon/ArtificialHorizon.qml
        ui/horizon/HorizonPanel.qml
        ui/horizon/TickMark.qml
    SOURCES
        src/core/Vehicle.h src/core/Vehicle.cpp
    RESOURCES
        ui/assets/plane.png
        ui/assets/plane_selected.png
        ui/assets/neostellar-logo.png
        ui/assets/marker.png
        ui/assets/padlock-unlock.png
        ui/assets/padlock.png
)

# Qt for iOS sets MACOSX_BUNDLE_GUI_IDENTIFIER automatically since Qt 6.1.
# If you are developing for iOS or macOS you should consider setting an
# explicit, fixed bundle idesntifier manually though.
set_target_properties(appAvaSYS2 PROPERTIES
#    MACOSX_BUNDLE_GUI_IDENTIFIER com.example.appAvaSYS2
    MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
    MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
    MACOSX_BUNDLE TRUE
    WIN32_EXECUTABLE TRUE
)

target_link_libraries(appAvaSYS2
    PRIVATE Qt6::Quick
    PRIVATE Qt6::Positioning
    PRIVATE Qt6::SerialPort
    PRIVATE Qt6::Network
    PRIVATE Qt6::Location
    PRIVATE Qt6::Qml
    PRIVATE Qt6::Gui
    PRIVATE OpenSSL::SSL
    PRIVATE OpenSSL::Crypto
    PRIVATE MAVLink::mavlink
    PRIVATE ${GST_APP_LIBRARIES} ${GST_VIDEO_LIBRARIES} ${GSTREAMER_LIBRARIES}
    PRIVATE ${OpenCV_LIBS}
)
target_include_directories(appAvaSYS2
        PRIVATE
        ${GSTREAMER_INCLUDE_DIRS}
        ${GST_APP_INCLUDE_DIRS}
        ${GST_VIDEO_INCLUDE_DIRS}
)
include(GNUInstallDirs)
install(TARGETS appAvaSYS2
    BUNDLE DESTINATION .
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)
