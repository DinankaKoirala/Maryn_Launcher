#pragma once
#include <QMainWindow>
#include <QNetworkAccessManager>
#include <QLabel>
#include <QTextEdit>
#include <QPushButton>
#include <QComboBox>
#include <QCheckBox>
#include <QStandardPaths>
#include "../core/download/Headers/VersionManifest.h"
#include "../core/download/Headers/VersionJsonParser.h"
#include "../core/download/Headers/AssetManager.h"
#include "../core/download/Headers/JavaManager.h"

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
    QPushButton *m_download;
    QPushButton *m_launch;
    QComboBox *m_versionSelection;
    QNetworkAccessManager *m_networkManager;
    QCheckBox *m_snapshotToggle;

    void populateVersionList();
};
