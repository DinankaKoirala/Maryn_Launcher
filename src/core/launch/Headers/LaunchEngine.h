#pragma once
#include <QString>
#include <QStringList>
#include <QObject>
#include <QMap>

class LaunchEngine : public QObject{
    Q_OBJECT

public:
    explicit LaunchEngine(QObject *parent = nullptr);

private:
    enum class Platform { Windows, MacOS, Linux};
    static Platform currentPlatform();
    static QString javaExecutableName();
    static QString classPathSeparator();
    QString buildClassPath(const QStringList &libraryPath , const QString &clientJarPath) const;
    QString ressolvePlaceholders(const QString &args , const QMAP<QString, QString> &vars) const;
    QString ressolveArgumentList(const QStringList &args , const QMAP<QString, QString> &vars) const;
    QStringList platformExtraJvmFlags() const;
};
