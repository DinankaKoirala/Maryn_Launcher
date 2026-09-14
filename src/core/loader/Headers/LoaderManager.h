#pragma once

#include <QObject>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QJsonDocument>
#include <QJsonArray>
#include <QJsonObject>
#include <QFile>
#include <QFileInfo>
#include <QDir>

enum class LoaderType { Vanilla, Fabric, Quilt };

class LoaderManager : public QObject {
    Q_OBJECT

public:
    explicit LoaderManager(QObject *parent = nullptr);
    void fetchLoaderVersions(const QString &mcVersion, LoaderType loader);
    void installLoader(const QString &mcVersion, const QString &loaderVersion, LoaderType loader, const QString &instanceDir, const QString &baseDir);

signals:
    void loaderVersionsFetched(QStringList versions);
    void installerFinished();
    void errorOccurred(QString message);

private:
    void downloadLoaderLibs(const QJsonArray &libraries, const QString &baseDir);

    QNetworkAccessManager *m_manager;
    int m_totalLibs = 0;
    int m_completedLibs = 0;
};