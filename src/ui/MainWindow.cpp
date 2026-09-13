#include "MainWindow.h"
#include "../core/download/Headers/DownloadManager.h"
#include "../core/launch/Headers/LaunchEngine.h"
#include "../core/instance/Headers/InstanceManager.h"
#include <QVBoxLayout>
#include <QHBoxLayout>

MainWindow::MainWindow(const QString &instanceName, const QString &version, const QList<VersionInfo> &versions, QWidget *parent)
    : QWidget(parent), m_instanceName(instanceName), m_instanceVersion(version), m_versions(versions)
{
    cacheDir = QStandardPaths::standardLocations(QStandardPaths::AppDataLocation).first();

    QVBoxLayout *mainLayout = new QVBoxLayout(this);

    QHBoxLayout *versionSelectionLayout = new QHBoxLayout(nullptr);
    m_versionSelection = new QComboBox(this);
    versionSelectionLayout->addWidget(m_versionSelection, 1);
    m_snapshotToggle = new QCheckBox("Snapshots", this);
    versionSelectionLayout->addWidget(m_snapshotToggle, 0);
    connect(m_snapshotToggle, &QCheckBox::checkStateChanged, this, &MainWindow::populateVersionList);
    mainLayout->addLayout(versionSelectionLayout);

    QHBoxLayout *buttonLayout = new QHBoxLayout(nullptr);
    m_download = new QPushButton("Download", this);
    m_launch = new QPushButton("Launch", this);
    m_launch->setObjectName("launchBtn");
    m_download->setObjectName("downloadBtn");
    buttonLayout->addWidget(m_download);
    buttonLayout->addWidget(m_launch);
    mainLayout->addLayout(buttonLayout);

    m_nameInput = new QLineEdit(this);
    m_nameInput->setPlaceholderText("Enter your name");
    m_nameInput->setMaxLength(16);
    mainLayout->addWidget(m_nameInput);

    m_logArea = new QTextEdit(this);
    m_logArea->setReadOnly(true);
    mainLayout->addWidget(m_logArea);

    connect(m_download, &QPushButton::clicked, this, &MainWindow::onDownloadClicked);
    connect(m_launch, &QPushButton::clicked, this, &MainWindow::onLaunchClicked);

    populateVersionList();

    // Select the instance's saved version
    int idx = m_versionSelection->findText(m_instanceVersion, Qt::MatchStartsWith);
    if (idx >= 0) m_versionSelection->setCurrentIndex(idx);

    // When version changes, update instance.json
    connect(m_versionSelection, &QComboBox::currentTextChanged, this, [this](const QString &text) {
        QString newVersion = text.split(' ').first();
        m_instanceVersion = newVersion;
        InstanceManager mgr;
        mgr.updateInstance(m_instanceName, QStandardPaths::writableLocation(QStandardPaths::AppDataLocation), newVersion);
    });
}

MainWindow::~MainWindow() {}

void MainWindow::populateVersionList() {
    m_versionSelection->clear();
    if (m_snapshotToggle->isChecked()) {
        for (const VersionInfo &v : m_versions)
            m_versionSelection->addItem(v.id + " (" + v.type + ")");
    } else {
        for (const VersionInfo &v : m_versions)
            if (v.type == "release")
                m_versionSelection->addItem(v.id + " (" + v.type + ")");
    }
}

void MainWindow::onDownloadClicked() {
    m_logArea->append("> Downloading...");
    m_logArea->append("> Parsing the version...");
    m_versionJsonParser = new VersionJsonParser(cacheDir, this);
    connect(m_versionJsonParser, &VersionJsonParser::finished, this, &MainWindow::onVersionJsonParsed);
    connect(m_versionJsonParser, &VersionJsonParser::errorOccurred, this, [this]() {
        m_logArea->append("> Error: failed to parse version");
    });
    QString selectedId = m_versionSelection->currentText().split(' ').first();
    for (const VersionInfo &version : m_versions) {
        if (version.id == selectedId)
            m_versionJsonParser->fetch(version.url);
    }
}

void MainWindow::onLaunchClicked() {
    QString baseDir = QStandardPaths::writableLocation(QStandardPaths::AppDataLocation);
    if (m_nameInput->text().trimmed().isEmpty()) {
        m_logArea->append("> Error: please enter a username.");
        return;
    }
    QString username = m_nameInput->text();
    m_logArea->append("> Launching...");

    m_launchEngine = new LaunchEngine(this);
    connect(m_launchEngine, &LaunchEngine::gameStarted, this, [this]() {
        m_logArea->append("> Game is running.");
    });
    connect(m_launchEngine, &LaunchEngine::gameFinished, this, [this](int exitCode) {
        m_logArea->append("> Game exited with code: " + QString::number(exitCode));
    });
    connect(m_launchEngine, &LaunchEngine::launchError, this, [this](const QString &msg) {
        m_logArea->append("> Launch error: " + msg);
    });
    connect(m_launchEngine, &LaunchEngine::logOutput, this, [this](const QString &line) {
        m_logArea->append(line);
    });

    m_launchEngine->launch(m_versionDetails, m_instanceName, username, offlineUuid(username), "0", baseDir);
}

void MainWindow::onVersionJsonParsed(VersionDetails details) {
    m_versionDetails = details;
    m_logArea->append("> Downloading libraries...");
    m_downloadManager = new DownloadManager(this);
    m_javaManager = new JavaManager(cacheDir, this);
    m_assetManager = new AssetManager(cacheDir, this);

    connect(m_downloadManager, &DownloadManager::finished, this, [this, details]() {
        m_logArea->append("> Libraries done. Downloading assets...");
        connect(m_assetManager, &AssetManager::finished, this, [this, details]() {
            m_logArea->append("> Assets done. Downloading Java...");
            connect(m_javaManager, &JavaManager::finished, this, [this]() {
                m_logArea->append("> All downloads complete.");
            });
            m_javaManager->download(details.javaRuntimeName);
        });
        m_assetManager->download(details.assetIndexUrl, details.assetIndexId);
    });

    m_downloadManager->download(details, m_instanceName);
}