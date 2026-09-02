#pragma once
#include <QMainWindow>
#include <QNetworkAccessManager>
#include <QLabel>
#include <QTextEdit>
#include <QPushButton>
#include <QComboBox>
#include <QCheckBox>
#include <QLineEdit>
#include <QStandardPaths>
#include "../core/download/Headers/VersionManifest.h"
#include "../core/download/Headers/VersionJsonParser.h"
#include "../core/download/Headers/AssetManager.h"
#include "../core/download/Headers/JavaManager.h"
#include "../core/download/Headers/VersionDetails.h"

class VersionManifest;
class DownloadManager;
class LaunchEngine;
class VersionJsonParser;
class JavaManager;
class AssetManager;

class MainWindow : public QMainWindow
{
    Q_OBJECT
public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void onDownloadClicked();
    void onLaunchClicked();
    void onManifestFetched(QList<VersionInfo> versions);
    void onVersionJsonParsed(VersionDetails details);

private:
    QString cacheDir;
    VersionManifest *m_versionManifest;
    DownloadManager *m_downloadManager;
    LaunchEngine *m_launchEngine;
    VersionJsonParser *m_versionJsonParser;
    AssetManager *m_assetManager;
    JavaManager *m_javaManager;
    QList<VersionInfo> m_versions;
    QLabel *m_statusLabel;
    QTextEdit *m_logArea;
    QLineEdit *m_nameInput;
    QPushButton *m_download;
    QPushButton *m_launch;
    QComboBox *m_versionSelection;
    QNetworkAccessManager *m_networkManager;
    QCheckBox *m_snapshotToggle;
    VersionDetails m_versionDetails;

    void populateVersionList();

    QString offlineUuid(const QString &username) {
        QByteArray hash = QCryptographicHash::hash(
            ("OfflinePlayer:" + username).toUtf8(),
                                                   QCryptographicHash::Md5
        );
        hash[6] = (hash[6] & 0x0f) | 0x30;
        hash[8] = (hash[8] & 0x3f) | 0x80;

        return QString("%1-%2-%3-%4-%5")
        .arg(hash.left(4).toHex())
        .arg(hash.mid(4,2).toHex())
        .arg(hash.mid(6,2).toHex())
        .arg(hash.mid(8,2).toHex())
        .arg(hash.right(6).toHex());
    }
};
