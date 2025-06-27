cmake_minimum_required(VERSION 3.16)

project(AvaSYS2 VERSION 0.1 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

find_package(Qt6 REQUIRED COMPONENTS Quick Network SerialPort Positioning Location)
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
)

qt_add_qml_module(appAvaSYS2
    URI AvaSYS2
    VERSION 1.0
    QML_FILES
        ui/Main.qml
        ui/components/RightPanel.qml
)

# Qt for iOS sets MACOSX_BUNDLE_GUI_IDENTIFIER automatically since Qt 6.1.
# If you are developing for iOS or macOS you should consider setting an
# explicit, fixed bundle identifier manually though.
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
    PRIVATE OpenSSL::SSL
    PRIVATE OpenSSL::Crypto
    PRIVATE MAVLink::mavlink
)

include(GNUInstallDirs)
install(TARGETS appAvaSYS2
    BUNDLE DESTINATION .
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)
