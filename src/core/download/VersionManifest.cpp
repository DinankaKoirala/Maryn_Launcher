#include "Headers/VersionManifest.h"
#include <QNetworkReply>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>

VersionManifest::VersionManifest(QObject *parent) : QObject(parent){
    m_manager = new QNetworkAccessManager(this);
}

void VersionManifest::fetch(){
    QUrl url("https://piston-meta.mojang.com/mc/game/version_manifest_v2.json");
    QNetworkRequest request(url);
    QNetworkReply *reply = m_manager->get(request);

    connect(reply , &QNetworkReply::finished,this,[this,reply](){
        onReplyFinished(reply);
    });
}

void VersionManifest::onReplyFinished(QNetworkReply *reply){
    reply->deleteLater();
    if(reply->error() != QNetworkReply::NoError){
        emit errorOccurred(reply->errorString());
        return;
    }
    QByteArray data =reply->readAll();
    QJsonParseError parseError;
    QJsonDocument doc = QJsonDocument::fromJson(data, &parseError);

    if(parseError.error != QJsonParseError::NoError){
        emit errorOccurred(parseError.errorString());
        return;
    }

    QJsonObject root = doc.object();
    QJsonArray versions = root["versions"].toArray();
    QList<VersionInfo> result;

    for(const QJsonValue &value : versions) {
        QJsonObject obj = value.toObject();

        VersionInfo info;
        info.id = obj["id"].toString();
        info.type = obj["type"].toString();
        info.url = obj["url"].toString();
        info.sha1 = obj["sha1"].toString();

        result.append(info);
    }

    emit finished(result);
}