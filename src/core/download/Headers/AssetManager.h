#pragma once
#include <QObject>
#include <QNetworkAccessManager>
#include <QNetworkReply>

class AssetManager : public QObject{
    Q_OBJECT

public:
    explicit AssetManager(const QString &cacheDir, QObject *parent = nullptr);
    void download(const QString &assetIndexUrl);


signals:
    void progress(int current, int total);
    void finished();
    void errorOccurred(const QString &message);
private:

    void downloadFile(const QString &url,const QString &savepath);
    void checkIfDone();

    QNetworkAccessManager *m_manager;
    QString m_cacheDir;
    int m_totalCount;
    int m_completedCount;
};
