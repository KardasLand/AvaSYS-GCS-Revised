//
// Created by kardasland on 7/5/25.
//

#ifndef TEKNOFESTQRCODEPROPERTY_H
#define TEKNOFESTQRCODEPROPERTY_H
#include <QObject>

class TeknofestQRCodeProperty : public QObject {
    Q_OBJECT
    Q_PROPERTY(double qrLatitude READ qrLatitude WRITE setQrLatitude NOTIFY qrLatitudeChanged)
    Q_PROPERTY(double qrLongitude READ qrLongitude WRITE setQrLongitude NOTIFY qrLongitudeChanged)
public:
    explicit TeknofestQRCodeProperty(QObject* parent = nullptr) : QObject(parent) {}

    double qrLatitude() { return m_qrLatitude; }
    double qrLongitude() { return m_qrLongitude; }

    void setQrLatitude(double latitude) {
        if (m_qrLatitude == latitude) return; // No change
        m_qrLatitude = latitude;
        emit qrLatitudeChanged(latitude);
    }

    void setQrLongitude(double longitude) {
        if (m_qrLongitude == longitude) return; // No change
        m_qrLongitude = longitude;
        emit qrLongitudeChanged(longitude);
    }
signals:
    void qrLatitudeChanged(double latitude);
    void qrLongitudeChanged(double longitude);
private:
    double m_qrLatitude;
    double m_qrLongitude;
};


#endif //TEKNOFESTQRCODEPROPERTY_H
