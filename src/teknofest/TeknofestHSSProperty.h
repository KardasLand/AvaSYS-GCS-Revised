//
// Created by kardasland on 7/5/25.
//

#ifndef TEKNOFESTHSSPROPERTY_H
#define TEKNOFESTHSSPROPERTY_H
#include <QObject>
#include <qqml.h>


class TeknofestHSSProperty : public QObject{
    Q_OBJECT
    Q_PROPERTY(double hssLatitude READ hssLatitude WRITE setHssLatitude NOTIFY hssLatitudeChanged)
    Q_PROPERTY(double hssLongitude READ hssLongitude WRITE setHssLongitude NOTIFY hssLongitudeChanged)
    Q_PROPERTY(double hssCircleRadius READ hssCircleRadius WRITE setHssCircleRadius NOTIFY hssCircleRadiusChanged)
public:
    explicit TeknofestHSSProperty(QObject* parent = nullptr) : QObject(parent), m_lat(0), m_long(0), m_radius(0) {
    }
    double hssLatitude() { return m_lat; }
    double hssLongitude() { return m_long; }
    double hssCircleRadius() { return m_radius; }

    void setHssLatitude(double lat) {
        if (m_lat == lat) return; // No change
        m_lat = lat;
        emit hssLatitudeChanged(lat);
    }
    void setHssLongitude(double lon) {
        if (m_long == lon) return; // No change
        m_long = lon;
        emit hssLongitudeChanged(lon);
    }
    void setHssCircleRadius(double radius) {
        if (m_radius == radius) return; // No change
        m_radius = radius;
        emit hssCircleRadiusChanged(radius);
    }
signals:
    void hssLatitudeChanged(double lat);
    void hssLongitudeChanged(double lon);
    void hssCircleRadiusChanged(double radius);
private:
    double m_lat;
    double m_long;
    double m_radius;
};



#endif //TEKNOFESTHSSPROPERTY_H
