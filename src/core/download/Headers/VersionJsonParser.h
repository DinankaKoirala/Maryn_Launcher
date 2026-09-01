#pragma once

#include<QObject>
#include<QNetworkAccessManager>
#include<QString>
#include<QList>
#include "VersionDetails.h"

class VersionJsonParser : public QObject{
    Q_OBJECT

    public: 
    explicit VersionJsonParser(const QString &cacheDir, QObject *parent = nullptr);
    void fetch(const QString &url);

    signals:
     void finished(VersionDetails details);
     void errorOccurred(QString message);

    private slots:
        void onReplyFinished(QNetworkReply *reply);

    private:
        QNetworkAccessManager *m_manager;
        QString m_cacheDir;
};
