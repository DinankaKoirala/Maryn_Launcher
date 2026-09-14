#pragma once

#include <QDir>
#include <QFile>
#include "VersionDetails.h"
#include <QDebug>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QDirIterator>

struct Instance {
    QString name;
    QString version;
    QString loader;
    QString loaderVersion;
};

class InstanceManager {
    public:
        void createInstance(QString &instanceName, const QString &baseDir, const QString &instanceVersion, const QString &loader, const QString &loaderVersion);
        QList<Instance> fetchInstancesList(const QString &baseDir);
        Instance getInstanceData(QString &instanceName, const QString &baseDir);
        void updateInstance(const QString &instanceName, const QString &baseDir, const QString &newVersion);
};