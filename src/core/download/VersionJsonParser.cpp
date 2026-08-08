#include "Headers/VersionJsonParser.h"
#include "Headers/VersionDetails.h"
#include <QNetworkReply>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>

VersionJsonParser::VersionJsonParser(QObject *parent):QObject(parent){
    m_manager = new QNetworkAccessManager(this);
}

void VersionJsonParser::fetch(const QString &url){
    QUrl qurl(url);
    QNetworkRequest request(url);
    QNetworkReply *reply = m_manager->get(request);

    connect(reply , &QNetworkReply::finished,this,[this,reply](){
        onReplyFinished(reply);
    });
}

void VersionJsonParser::onReplyFinished(QNetworkReply *reply){
    reply->deleteLater();
    if(reply->error() != QNetworkReply::NoError){
        emit errorOccurred(reply->errorString());
        return;
    }
    QByteArray data = reply->readAll();
    QJsonParseError parseError;
    QJsonDocument doc = QJsonDocument::fromJson(data , &parseError);

    if(parseError.error != QJsonParseError::NoError){
        emit errorOccurred(parseError.errorString());
        return;
    }

    QJsonObject root = doc.object();
    VersionDetails details;
    details.mainClass = root["mainClass"].toString();
    details.javaVersion = root["javaVersion"].toObject()["majorVersion"].toInt();

    QJsonObject clientDownload = root["downloads"].toObject()["client"].toObject();
    details.clientUrl  = clientDownload["url"].toString();
    details.clientSha1 = clientDownload["sha1"].toString();

    QJsonObject assetIndex = root["assetIndex"].toObject();
    details.assetIndexUrl = assetIndex["url"].toString();
    details.assetIndexId  = assetIndex["id"].toString();

    QJsonArray librariesArray = root["libraries"].toArray();
    for(const QJsonValue &value : librariesArray){
        QJsonObject libObj = value.toObject();

        if(!libObj["downloads"].toObject().contains("artifact")) {
            continue;
        }
        QJsonObject artifact = libObj["downloads"].toObject()["artifact"].toObject();

        LibraryInfo lib;
        lib.url = artifact["url"].toString();
        lib.sha1 = artifact["sha1"].toString();
        lib.path = artifact["path"].toString();

        details.libraries.append(lib);
    }
    QJsonArray gameArgs = root["arguments"].toObject()["game"].toArray();
    for(const QJsonValue &value : gameArgs){
        if(value.isString()){
            details.gameArgs.append(value.toString());
        }
    }
    QJsonArray jvmArgs = root["arguments"].toObject()["jvm"].toArray();
    for(const QJsonValue &value : jvmArgs){
        if(value.isString()){
            details.jvmArgs.append(value.toString());
        }
    }
    
    emit finished(details);
}