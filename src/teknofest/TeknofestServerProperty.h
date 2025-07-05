//
// Created by kardasland on 7/5/25.
//

#ifndef TEKNOFESTSERVERPROPERTIES_H
#define TEKNOFESTSERVERPROPERTIES_H
#include <QObject>
#include <vector>
#include "TeknofestAuthProperty.h"
#include "TeknofestQRCodeProperty.h"
#include "TeknofestHSSProperty.h"

class TeknofestServerProperties : public QObject {
    Q_OBJECT
    Q_PROPERTY(TeknofestAuthProperty* teknofestAuthProperty READ getTeknofestAuthProperty WRITE setTeknofestAuthProperty NOTIFY teknofestAuthPropertyChanged)
    Q_PROPERTY(TeknofestQRCodeProperty* teknofestQRCodeProperty READ getTeknofestQRCodeProperty WRITE setTeknofestQRCodeProperty NOTIFY teknofestQRCodePropertyChanged)
    Q_PROPERTY(QString url READ getUrl WRITE setUrl NOTIFY urlChanged)
    Q_PROPERTY(QString takimid READ getTakimid WRITE setTakimid NOTIFY takimidChanged)
    Q_PROPERTY(QByteArray session_id READ getSessionId WRITE setSessionId NOTIFY sessionIdChanged)
    Q_PROPERTY(QList<TeknofestHSSProperty*> teknofestHSSProperties READ getTeknofestHSSProperties WRITE setTeknofestHSSProperties NOTIFY teknofestHSSPropertiesChanged)
public:
    explicit TeknofestServerProperties(QObject* parent = nullptr) : QObject(parent){
        qRegisterMetaType<QList<TeknofestHSSProperty*>>("QList<TeknofestHSSProperty*>");
    }

    TeknofestAuthProperty* getTeknofestAuthProperty() const { return m_authProperty; }

    void setTeknofestAuthProperty(TeknofestAuthProperty* authProperty) {
        if (m_authProperty == authProperty) return; // No change
        m_authProperty = authProperty;
        emit teknofestAuthPropertyChanged();
    }

    TeknofestQRCodeProperty* getTeknofestQRCodeProperty() const { return m_qrCodeProperty; }
    void setTeknofestQRCodeProperty(TeknofestQRCodeProperty* qrCodeProperty) {
        if (m_qrCodeProperty == qrCodeProperty) return; // No change
        m_qrCodeProperty = qrCodeProperty;
        emit teknofestQRCodePropertyChanged();
    }

    QString getUrl() const { return url; }
    Q_INVOKABLE void setUrl(const QString &url) {
        if (this->url == url) return; // No change
        this->url = url;
        emit urlChanged();
    }

    QString getTakimid() const { return takimid; }
    void setTakimid(const QString &takimid) {
        if (this->takimid == takimid) return; // No change
        this->takimid = takimid;
        emit takimidChanged();
    }

    QByteArray getSessionId() const { return session_id; }
    void setSessionId(const QByteArray &sessionId) {
        if (this->session_id == sessionId) return; // No change
        this->session_id = sessionId;
        emit sessionIdChanged();
    }

    QList<TeknofestHSSProperty *> getTeknofestHSSProperties() const {
        return m_hssProperty;
    }
    void setTeknofestHSSProperties(QList<TeknofestHSSProperty *> properties) {
         // check contents for changes
        if (m_hssProperty == properties) return; // No change
        if (m_hssProperty.isEmpty()) {
            // If the current list is empty, just set the new list
            m_hssProperty = properties;
            emit teknofestHSSPropertiesChanged();
            return;
        }
        // todo EXPERIMENTAL
        // check if new list's contents are different
        for (int i = 0; i < m_hssProperty.size() && i < properties.size(); ++i) {
            if (m_hssProperty[i]->hssLatitude() != properties[i]->hssLatitude() ||
                m_hssProperty[i]->hssLongitude() != properties[i]->hssLongitude() ||
                m_hssProperty[i]->hssCircleRadius() != properties[i]->hssCircleRadius()) {
                // Contents are different
                m_hssProperty = properties;
                emit teknofestHSSPropertiesChanged();
                break;
            }
        }
        // If we reach here, it means contents are the same
        return;
    }

signals:
    void teknofestAuthPropertyChanged();
    void teknofestQRCodePropertyChanged();
    void urlChanged();
    void takimidChanged();
    void sessionIdChanged();
    void teknofestHSSPropertiesChanged();
private:
    TeknofestAuthProperty* m_authProperty;
    TeknofestQRCodeProperty* m_qrCodeProperty;
    QString url;
    QString takimid;
    QByteArray session_id;
    QList<TeknofestHSSProperty*> m_hssProperty = QList<TeknofestHSSProperty*>();
};

#endif //TEKNOFESTSERVERPROPERTIES_H
