//
// Created by kardasland on 6/27/25.
//

#include "TeknofestClient.h"

TeknofestClient::TeknofestClient(QObject *parent) : QObject(parent) {
    qRegisterMetaType<QJsonArray>("QJsonArray");
}

void TeknofestClient::login(const QString &url, const QString &username, const QString &password) {
}

void TeknofestClient::startFetchingTelemetry() {
}

void TeknofestClient::stopFetchingTelemetry() {
}

void TeknofestClient::onLoginReply(QNetworkReply *reply) {
}

void TeknofestClient::onTelemetryReply(QNetworkReply *reply) {
}

void TeknofestClient::fetchTelemetry() {
}
