#pragma once
#include <QObject>
#include <QNetworkAccessManager>
#include <QNetworkReply>

class JavaManager : public QObject
{
    Q_OBJECT

public:
    explicit JavaManager(const QString &cacheDir, QObject *parent = nullptr);
    void download(const QString &runtimeName);

signals:
    void progress(int current, int total);
    void finished();
    void errorOccurred(const QString &message);

private:
    void fetchManifest(const QString &manifestUrl, const QString &runtimeName);
    void downloadFile(const QString &url, const QString &savePath);
    void checkIfDone();

    static QString platformKey();

    QNetworkAccessManager *m_manager;
    QString m_cacheDir;
    int m_totalCount;
    int m_completedCount;
};
