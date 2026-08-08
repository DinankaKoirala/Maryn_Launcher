#pragma once

#include <QObject>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QDir>
#include <QFile>
#include "VersionDetails.h"

class DownloadManager : public QObject{
    Q_OBJECT

 public:
    explicit DownloadManager(QObject *parent = nullptr);
    void download(const VersionDetails &details, const QString &instanceName);

 signals:
    void progress(int current, int total);
    void finished();
    void errorOccurred(QString message);

 private:
    void downloadFile(const QString &url,const QString &savepath);
    void onFileDownloaded(QNetworkReply *reply,const QString &savepath );
    void checkIfDone();

    QNetworkAccessManager *m_manager;
    QString m_baseDir;
    int m_totalCount;
    int m_completedCount;

};