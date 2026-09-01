#include "Headers/JavaManager.h"
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QFile>
#include <QFileInfo>
#include <QDir>

JavaManager::JavaManager(const QString &cacheDir, QObject *parent)
: QObject(parent), m_cacheDir(cacheDir), m_totalCount(0), m_completedCount(0)
{
    m_manager = new QNetworkAccessManager(this);
}

QString JavaManager::platformKey()
{
    #if defined(Q_OS_WIN)
    return QStringLiteral("windows-x64");
    #elif defined(Q_OS_MACOS)
    return QStringLiteral("mac-os");
    #else
    return QStringLiteral("linux");
    #endif
}

void JavaManager::download(const QString &runtimeName)
{
    const QString url = QStringLiteral(
        "https://launchermeta.mojang.com/v1/products/java-runtime/"
        "2ec0cc96c44e5a76b9c8b7c39df7210883d12871/all.json"
    );

    QNetworkReply *reply = m_manager->get(QNetworkRequest(QUrl(url)));

    connect(reply, &QNetworkReply::finished, this, [this, reply, runtimeName]() {
        reply->deleteLater();

        if (reply->error() != QNetworkReply::NoError) {
            emit errorOccurred(reply->errorString());
            return;
        }

        QJsonDocument doc = QJsonDocument::fromJson(reply->readAll());
        QJsonObject root = doc.object();

        // Pick our platform, then our runtime inside it
        QJsonArray runtimeArray = root[platformKey()]
        .toObject()[runtimeName]
        .toArray();

        if (runtimeArray.isEmpty()) {
            emit errorOccurred("Runtime not found for this platform.");
            return;
        }

        // The manifest URL is inside the first entry
        QString manifestUrl = runtimeArray[0]
        .toObject()["manifest"]
        .toObject()["url"]
        .toString();

        fetchManifest(manifestUrl, runtimeName);
    });
}

void JavaManager::fetchManifest(const QString &manifestUrl, const QString &runtimeName)
{
    QNetworkReply *reply = m_manager->get(QNetworkRequest(QUrl(manifestUrl)));

    connect(reply, &QNetworkReply::finished, this, [this, reply, runtimeName]() {
        reply->deleteLater();

        if (reply->error() != QNetworkReply::NoError) {
            emit errorOccurred(reply->errorString());
            return;
        }

        QJsonDocument doc = QJsonDocument::fromJson(reply->readAll());
        QJsonObject files = doc.object()["files"].toObject();

        // Count only downloadable files first
        m_totalCount = 0;
        m_completedCount = 0;

        for (auto it = files.constBegin(); it != files.constEnd(); ++it) {
            QJsonObject fileObj = it.value().toObject();
            if (fileObj["type"].toString() == "file") {
                m_totalCount++;
            }
        }

        if (m_totalCount == 0) {
            emit finished();
            return;
        }

        // Now kick off every download
        for (auto it = files.constBegin(); it != files.constEnd(); ++it) {
            QJsonObject fileObj = it.value().toObject();

            if (fileObj["type"].toString() != "file") {
                continue;  // skip directories
            }

            QString relativePath = it.key();
            QString savePath = QDir::cleanPath(
                m_cacheDir + "/runtime/" + runtimeName + "/" + relativePath
            );

            QString url = fileObj["downloads"]
            .toObject()["raw"]
            .toObject()["url"]
            .toString();

            downloadFile(url, savePath);
        }
    });
}


void JavaManager::downloadFile(const QString &url, const QString &savePath)
{
    QNetworkReply *reply = m_manager->get(QNetworkRequest(QUrl(url)));

    connect(reply, &QNetworkReply::finished, this, [this, reply, savePath]() {
        reply->deleteLater();

        if (reply->error() != QNetworkReply::NoError) {
            emit errorOccurred(reply->errorString());
            return;
        }

        // Make sure the directory exists before writing
        QFileInfo fileInfo(savePath);
        QDir().mkpath(fileInfo.absolutePath());

        QFile file(savePath);
        if (file.open(QIODevice::WriteOnly)) {
            file.write(reply->readAll());
            file.close();
        } else {
            emit errorOccurred("Could not write file: " + savePath);
        }

        checkIfDone();
    });
}

void JavaManager::checkIfDone()
{
    m_completedCount++;
    emit progress(m_completedCount, m_totalCount);

    if (m_completedCount == m_totalCount) {
        emit finished();
    }
}


