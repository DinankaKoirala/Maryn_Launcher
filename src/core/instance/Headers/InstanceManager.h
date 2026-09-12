#pragma once

#include <QDir>
#include <QFile>
#include "VersionDetails.h"

struct Instance{
    QString name;
    QString version;
};


class InstanceManager{
    public:
        void createInstance(QString instanceName , const QString &baseDir , const QString &instanceVersion);
        QList<Instance> fetchInstancesList(const QString &baseDir);
        Instance getInstanceData(QString instanceName , const QString &baseDir);

};