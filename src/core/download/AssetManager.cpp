#include "Headers/AssetManager.h"
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QFile>
#include <QFileInfo>
#include <QDir>

AssetManager::AssetManager(const QString &baseDir, QObject *parent)
: QObject(parent), m_baseDir(baseDir), m_totalCount(0), m_completedCount(0)
{
    m_manager = new QNetworkAccessManager(this);
}

void AssetManager::download(const QString &assetIndexUrl, const QString &assetIndexId){
    QNetworkReply *reply = m_manager->get(QNetworkRequest(QUrl(assetIndexUrl)));

    connect(reply, &QNetworkReply::finished, this, [this,reply, assetIndexId, assetIndexUrl](){
        reply->deleteLater();

        if(reply->error() != QNetworkReply::NoError){
            emit errorOccurred(reply->errorString());
            return;
        }

        QJsonDocument doc = QJsonDocument::fromJson(reply->readAll());
        QJsonObject  objects = doc.object()["objects"].toObject();

        m_totalCount = 0;
        m_completedCount = 0;

        for (auto it = objects.constBegin(); it != objects.constEnd(); ++it) {
            m_totalCount++;
        }

        QString indexPath = m_baseDir + "/assets/indexes/" + assetIndexId + ".json";
        downloadIndex(assetIndexUrl,indexPath );


        for (auto it = objects.constBegin(); it != objects.constEnd(); ++it){
            QJsonObject fileObj = it.value().toObject();
            QString hash = fileObj["hash"].toString();
            QString twoChars = hash.left(2);

            QString url = "https://resources.download.minecraft.net/" + twoChars + "/" + hash;
            QString savePath = QDir::cleanPath(m_baseDir + "/assets/objects/" + twoChars + "/" + hash);

            downloadFile(url, savePath);
        }
    });
}

void AssetManager::downloadFile(const QString &url,const QString &savepath){
    QNetworkReply *reply = m_manager->get(QNetworkRequest(QUrl(url)));
    connect(reply, &QNetworkReply::finished, this, [this,reply,savepath](){
        reply->deleteLater();
        if(reply->error() != QNetworkReply::NoError) {
            emit errorOccurred(reply->errorString());
            return;
        }
        QFileInfo fileInfo(savepath);
        QDir().mkpath(fileInfo.absolutePath());

        QFile file(savepath);
        if (file.open(QIODevice::WriteOnly)) {
            file.write(reply->readAll());
            file.close();
        }

        checkIfDone();
    });
}

void AssetManager::checkIfDone()
{
    m_completedCount++;
    qDebug() << m_completedCount <<"/"<< m_totalCount;
    emit progress(m_completedCount, m_totalCount);

    if (m_completedCount == m_totalCount) {
        emit finished();
    }
}


void AssetManager::downloadIndex(const QString &url,const QString &savepath){
    QNetworkReply *reply = m_manager->get(QNetworkRequest(QUrl(url)));
    connect(reply, &QNetworkReply::finished, this, [this,reply,savepath](){
        reply->deleteLater();
        if(reply->error() != QNetworkReply::NoError) {
            emit errorOccurred(reply->errorString());
            return;
        }
        QFileInfo fileInfo(savepath);
        QDir().mkpath(fileInfo.absolutePath());

        QFile file(savepath);
        if (file.open(QIODevice::WriteOnly)) {
            file.write(reply->readAll());
            file.close();
        }
    });
}
