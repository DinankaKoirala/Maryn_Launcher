#pragma once

#include <QString>
#include <QList>

struct LibraryInfo{
    QString url;
    QString sha1;
    QString path;
};

struct VersionDetails{
    QString clientUrl;
    QString clientSha1;
    QString mainClass;
    QString assetIndexUrl;
    QString assetIndexId;
    int javaVersion;
    QList<LibraryInfo> libraries;
    QList<QString> gameArgs;
    QList<QString> jvmArgs;
};