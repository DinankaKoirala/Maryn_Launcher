#include "Headers/DownloadManager.h"
#include <QStandardPaths>
#include <QDebug>

DownloadManager::DownloadManager(QObject *parent):QObject(parent){
    m_manager = new QNetworkAccessManager(this);
    m_totalCount = 0;
    m_completedCount = 0;
    
    QString appData = QStandardPaths::writableLocation(QStandardPaths::AppDataLocation);
    m_baseDir = appData;
    qDebug() << "AppData path:" << appData;

    QDir dir;
    dir.mkpath(m_baseDir + "/libraries");
}

void DownloadManager::download(const VersionDetails &details, const QString &instanceName){
    m_completedCount = 0;
    m_totalCount = 1 + details.libraries.size();

    QString instanceDir = m_baseDir + "/instances/" + instanceName;
    QDir dir;
    dir.mkpath(instanceDir);

    QString clientPath = instanceDir + "/client.jar";
    downloadFile(details.clientUrl, clientPath);
    for(const LibraryInfo &lib : details.libraries){
        QString libPath = m_baseDir + "/libraries/" + lib.path;
        QFileInfo fileInfo(libPath);
        dir.mkpath(fileInfo.absolutePath());

    if(QFile::exists(libPath)){
        m_completedCount++;
        emit progress(m_completedCount,m_totalCount);
        continue;
    }
    downloadFile(lib.url, libPath);
    }
    checkIfDone();
}

void DownloadManager::downloadFile(const QString &url, const QString &savePath){
    QUrl qurl(url);
    QNetworkRequest request(qurl);
    QNetworkReply *reply = m_manager->get(request);

    connect(reply, &QNetworkReply::finished,this,[this,reply,savePath](){
        onFileDownloaded(reply,savePath);
    });
}

void DownloadManager::onFileDownloaded(QNetworkReply *reply, const QString &savePath){
    reply->deleteLater();
    if (reply->error() != QNetworkReply::NoError) {
        emit errorOccurred(reply->errorString());
        return;
    }

    QByteArray data = reply->readAll();
    QFile file(savePath);

    if(!file.open(QIODevice::WriteOnly)){
        emit errorOccurred("Failed to open file for writing: " + savePath);
        return;
    }
    file.write(data);
    file.close();
    m_completedCount++;
    emit progress(m_completedCount, m_totalCount);

    checkIfDone();
}

void DownloadManager::checkIfDone()
{
    if (m_completedCount >= m_totalCount) {
        emit finished();
    }
}
