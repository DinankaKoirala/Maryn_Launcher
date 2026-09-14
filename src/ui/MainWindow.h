#pragma once
#include <QWidget>
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
#include "../core/loader/Headers/LoaderManager.h"

class DownloadManager;
class LaunchEngine;
class VersionJsonParser;
class JavaManager;
class AssetManager;

class MainWindow : public QWidget
{
    Q_OBJECT
public:
    explicit MainWindow(const QString &instanceName, const QString &version, const QList<VersionInfo> &versions, QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void onDownloadClicked();
    void onLaunchClicked();
    void onVersionJsonParsed(VersionDetails details);

private:
    QString m_instanceName;
    QString m_instanceVersion;
    QString cacheDir;
    DownloadManager *m_downloadManager = nullptr;
    LaunchEngine *m_launchEngine = nullptr;
    VersionJsonParser *m_versionJsonParser = nullptr;
    AssetManager *m_assetManager = nullptr;
    JavaManager *m_javaManager = nullptr;
    LoaderManager *m_loaderManager = nullptr;
    QList<VersionInfo> m_versions;
    QTextEdit *m_logArea;
    QLineEdit *m_nameInput;
    QPushButton *m_download;
    QPushButton *m_launch;
    QComboBox *m_versionSelection;
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