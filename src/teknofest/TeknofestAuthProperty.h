//
// Created by kardasland on 7/5/25.
//

#ifndef TEKNOFESTAUTHPROPERTY_H
#define TEKNOFESTAUTHPROPERTY_H
#include <QObject>

class TeknofestAuthProperty : public QObject {
    Q_OBJECT
    Q_PROPERTY(QString username READ getUsername WRITE setUsername NOTIFY usernameChanged)
    Q_PROPERTY(QString password READ getPassword WRITE setPassword NOTIFY passwordChanged)
public:
    explicit TeknofestAuthProperty(QObject* parent = nullptr) : QObject(parent), m_username(""), m_password("") {}

    QString getUsername() { return m_username; }
    Q_INVOKABLE void setUsername(QString username) {
        if (m_username == username) return; // No change
        m_username = username;
        emit usernameChanged(username);
    }

    QString getPassword() { return m_password; }
    Q_INVOKABLE void setPassword(QString password) {
        if (m_password == password) return; // No change
        m_password = password;
        emit passwordChanged(password);
    }
signals:
    void usernameChanged(QString& username);
    void passwordChanged(QString& password);
private:
    QString m_username;
    QString m_password;
};
#endif //TEKNOFESTAUTHPROPERTY_H
