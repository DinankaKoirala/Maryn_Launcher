#include "Headers/LoaderManager.h"
#include <QDebug>

LoaderManager::LoaderManager(QObject *parent)
    : QObject(parent), m_manager(new QNetworkAccessManager(this))
{
}

void LoaderManager::fetchLoaderVersions(const QString &mcVersion, LoaderType loader) {
    if (loader == LoaderType::Vanilla) {
        emit loaderVersionsFetched({});
        return;
    }

    QString url;
    if (loader == LoaderType::Fabric)
        url = "https://meta.fabricmc.net/v2/versions/loader/" + mcVersion;
    else if (loader == LoaderType::Quilt)
        url = "https://meta.quiltmc.org/v3/versions/loader/" + mcVersion;

    QNetworkRequest req{QUrl(url)};
    QNetworkReply *reply = m_manager->get(req);

    connect(reply, &QNetworkReply::finished, this, [this, reply]() {
        reply->deleteLater();
        if (reply->error() != QNetworkReply::NoError) {
            emit errorOccurred(reply->errorString());
            return;
        }
        QJsonDocument doc = QJsonDocument::fromJson(reply->readAll());
        if (!doc.isArray()) {
            emit errorOccurred("Unexpected response format");
            return;
        }
        QStringList versions;
        for (const QJsonValue &val : doc.array()) {
            QString version = val.toObject()["loader"].toObject()["version"].toString();
            if (!version.isEmpty())
                versions.append(version);
        }
        emit loaderVersionsFetched(versions);
    });
}

void LoaderManager::installLoader(const QString &mcVersion, const QString &loaderVersion, LoaderType loader, const QString &instanceDir, const QString &baseDir) {
    if (loader == LoaderType::Vanilla) {
        emit installerFinished();
        return;
    }

    QString profilePath = instanceDir + (loader == LoaderType::Fabric ? "/fabric-profile.json" : "/quilt-profile.json");

    if (QFile::exists(profilePath)) {
        emit installerFinished();
        return;
    }

    QString url;
    if (loader == LoaderType::Fabric)
        url = QString("https://meta.fabricmc.net/v2/versions/loader/%1/%2/profile/json")
                  .arg(mcVersion, loaderVersion);
    else if (loader == LoaderType::Quilt)
        url = QString("https://meta.quiltmc.org/v3/versions/loader/%1/%2/profile/json")
                  .arg(mcVersion, loaderVersion);

    QNetworkRequest req{QUrl(url)};
    QNetworkReply *reply = m_manager->get(req);

    connect(reply, &QNetworkReply::finished, this, [this, reply, profilePath, baseDir, instanceDir]() {
        reply->deleteLater();
        if (reply->error() != QNetworkReply::NoError) {
            emit errorOccurred(reply->errorString());
            return;
        }

        QByteArray data = reply->readAll();
        QJsonDocument doc = QJsonDocument::fromJson(data);
        if (!doc.isObject()) {
            emit errorOccurred("Invalid profile JSON");
            return;
        }

        // Save profile JSON
        QFile profileFile(profilePath);
        if (profileFile.open(QIODevice::WriteOnly)) {
            profileFile.write(doc.toJson(QJsonDocument::Indented));
            profileFile.close();
        }

        downloadLoaderLibs(doc.object()["libraries"].toArray(), baseDir);
    });
}

void LoaderManager::downloadLoaderLibs(const QJsonArray &libraries, const QString &baseDir) {
    QList<QPair<QString, QString>> toDownload;

    for (const QJsonValue &val : libraries) {
        QJsonObject lib = val.toObject();
        QString name = lib["name"].toString();

        QStringList parts = name.split(':');
        if (parts.size() < 3) continue;

        QString group    = parts[0].replace('.', '/');
        QString artifact = parts[1];
        QString version  = parts[2];
        QString path     = QString("%1/%2/%3/%2-%3.jar").arg(group, artifact, version);
        QString savePath = baseDir + "/libraries/" + path;

        QString baseUrl = lib["url"].toString();
        if (baseUrl.isEmpty())
            baseUrl = "https://repo1.maven.org/maven2/";
        if (!baseUrl.endsWith('/'))
            baseUrl += '/';

        if (QFile::exists(savePath)) continue;
        toDownload.append({baseUrl + path, savePath});
    }

    m_totalLibs = toDownload.size();
    m_completedLibs = 0;

    if (m_totalLibs == 0) {
        emit installerFinished();
        return;
    }

    for (const auto &pair : toDownload) {
        QNetworkRequest req{QUrl(pair.first)};
        QNetworkReply *reply = m_manager->get(req);
        QString savePath = pair.second;

        connect(reply, &QNetworkReply::finished, this, [this, reply, savePath]() {
            reply->deleteLater();
            if (reply->error() != QNetworkReply::NoError) {
                emit errorOccurred(reply->errorString());
                return;
            }

            QFileInfo fi(savePath);
            QDir().mkpath(fi.absolutePath());

            QFile file(savePath);
            if (file.open(QIODevice::WriteOnly)) {
                file.write(reply->readAll());
                file.close();
            }

            m_completedLibs++;
            if (m_completedLibs >= m_totalLibs)
                emit installerFinished();
        });
    }
}