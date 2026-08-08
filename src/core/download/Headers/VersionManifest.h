#pragma once

#include<QObject>
#include<QNetworkAccessManager>
#include<QString>
#include<QList>

struct VersionInfo{
    QString id;
    QString type;
    QString url;
    QString sha1;
};

class VersionManifest : public QObject{
    Q_OBJECT

    public: 
    explicit VersionManifest(QObject *parent = nullptr);
    void fetch();

    signals:
     void finished(QList<VersionInfo> versions);
     void errorOccurred(QString message);

    private slots:
        void onReplyFinished(QNetworkReply *reply);

    private:
        QNetworkAccessManager *m_manager;
};