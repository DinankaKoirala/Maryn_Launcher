#pragma once
#include <QString>
#include <QStringList>
#include <QObject>
#include <QMap>
#include <QDir>
#include <QProcess>
#include "VersionDetails.h"
#include <QStandardPaths>

class VersionJsonParser;


class LaunchEngine : public QObject{
    Q_OBJECT

public:
    explicit LaunchEngine(QObject *parent = nullptr);
    void launch(const VersionDetails &details, const QString &instanceName, const QString &playerName, const QString &playerUUID, const QString &accessToken, const QString &gameDir);

signals:
    void gameStarted();
    void gameFinished(int exitCode);
    void launchError(const QString &message);
    void logOutput(const QString &line);

private:
    enum class Platform { Windows, MacOS, Linux};
    static Platform currentPlatform();
    static QString javaExecutableName();
    static QString classPathSeparator();
    QString buildClassPath(const QStringList &libraryPath , const QString &clientJarPath) const;
    QString resolvePlaceholders(const QString &args , const QMap<QString, QString> &vars) const;
    QStringList resolveArgumentList(const QStringList &args , const QMap<QString, QString> &vars) const;
    QStringList platformExtraJvmFlags() const;
};
