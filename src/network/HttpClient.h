#ifndef HTTPCIENT_H
#define HTTPCIENT_H

#include <QObject>
#include <QNetworkAccessManager>
#include <QJsonObject>
#include <QNetworkReply>

class HttpClient : public QObject
{
    Q_OBJECT

public:
    explicit HttpClient(QObject* parent = nullptr);
    ~HttpClient();

public slots:
    void sendLoginRequest(const QString& url, const QString& username, const QString& password);
    void sendTelemetryRequest(const QString& url, const QByteArray& sessionId, const QJsonObject& telemetryData);

    signals:
        void loginFinished(int teamId, const QByteArray& sessionId, const QString& errorString);
    void telemetryResponseReceived(const QByteArray& responseData, const QString& errorString);

private slots:
    void onLoginReplyFinished();
    void onTelemetryReplyFinished();

private:
    QNetworkAccessManager* m_networkManager;
};

#endif // HTTPCIENT_H