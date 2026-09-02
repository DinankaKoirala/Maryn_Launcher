#include "MainWindow.h"
#include "../core/download/Headers/VersionManifest.h"
#include "../core/download/Headers/DownloadManager.h"
#include "../core/launch/Headers/LaunchEngine.h"
#include <QVBoxLayout>
#include <QStatusBar>

MainWindow::MainWindow(QWidget *parent)
: QMainWindow(parent)
{
 setWindowTitle("Maryn Launcher");
 setMinimumSize(520, 360);

 QWidget *centralWidget = new QWidget(this);
 setCentralWidget(centralWidget);

 QVBoxLayout *mainLayout = new QVBoxLayout(centralWidget);

 QHBoxLayout *versionSelectionLayout = new QHBoxLayout(nullptr);
 m_versionSelection = new QComboBox(this);
 versionSelectionLayout->addWidget(m_versionSelection, 1);
 m_snapshotToggle = new QCheckBox(this);
 m_snapshotToggle->setText("Snapshots");
 versionSelectionLayout->addWidget(m_snapshotToggle, 0);
 connect(m_snapshotToggle, &QCheckBox::checkStateChanged, this, &MainWindow::populateVersionList);
 mainLayout->addLayout(versionSelectionLayout);

 QHBoxLayout *buttonLayout = new QHBoxLayout(nullptr);

 m_download = new QPushButton("Download",this);
 buttonLayout->addWidget(m_download);
 m_launch = new QPushButton("Launch",this);
 buttonLayout->addWidget(m_launch);
 mainLayout->addLayout(buttonLayout);

 m_nameInput = new QLineEdit(this);
 m_nameInput->setPlaceholderText("Enter your name");
 m_nameInput->setMaxLength(16);
 mainLayout->addWidget(m_nameInput);
 m_logArea = new QTextEdit(this);
 m_logArea->setReadOnly(true);
 mainLayout->addWidget(m_logArea);

 statusBar()->showMessage("idle");



 m_launch->setObjectName("launchBtn");
 m_download->setObjectName("downloadBth");
 connect(m_download, &QPushButton::clicked, this, &MainWindow::onDownloadClicked);
 connect(m_launch, &QPushButton::clicked, this, &MainWindow::onLaunchClicked);

 cacheDir = QStandardPaths::standardLocations(QStandardPaths::AppDataLocation).first();


m_versionManifest = new VersionManifest(this);
connect(m_versionManifest, &VersionManifest::finished, this, &MainWindow::onManifestFetched);
connect(m_versionManifest, &VersionManifest::errorOccurred, this, [this]() {
    m_logArea->append("> Error: failed to fetch version manifest.");
    statusBar()->showMessage("error");
});
m_versionManifest->fetch();
}

MainWindow::~MainWindow() {

}

void MainWindow::onDownloadClicked(){
    m_logArea->append(">Downloading...");
    m_logArea->append(">Parsing the version...");
    statusBar()->showMessage("downloading");
    m_versionJsonParser = new VersionJsonParser(cacheDir, this);

    connect(m_versionJsonParser, &VersionJsonParser::finished, this, &MainWindow::onVersionJsonParsed);
    connect(m_versionJsonParser, &VersionJsonParser::errorOccurred, this, [this]() {
        m_logArea->append("> Error: failed to parse version");
        statusBar()->showMessage("error");
    });
    QString selectedId = m_versionSelection->currentText().split(' ').first();
    for(const VersionInfo &version : m_versions){
        if(version.id == selectedId){
           m_versionJsonParser->fetch(version.url);
        }
    }

}

void MainWindow::onLaunchClicked(){
    QString baseDir = QStandardPaths::writableLocation(QStandardPaths::AppDataLocation);
    m_logArea->append(">Launching...");
    statusBar()->showMessage("launching");
    QString username = m_nameInput->text();
    if(m_nameInput->text().trimmed().isEmpty()){
        m_logArea->append("> Error: please enter a username.");
        return;
    }

    m_launchEngine = new LaunchEngine(this);
    connect(m_launchEngine, &LaunchEngine::gameStarted, this, [this]() {
        m_logArea->append("> Game is running.");
        statusBar()->showMessage("Game Running");
    });

    connect(m_launchEngine, &LaunchEngine::gameFinished, this, [this](int exitCode) {
        m_logArea->append("> Game exited with code: " + QString::number(exitCode));
        statusBar()->showMessage("idle");
    });

    connect(m_launchEngine, &LaunchEngine::launchError, this, [this](const QString &message) {
        m_logArea->append("> Launch error: " + message);
        statusBar()->showMessage("error");
    });

    connect(m_launchEngine, &LaunchEngine::logOutput, this, [this](const QString &line) {
        m_logArea->append(line);
    });

    m_launchEngine->launch(m_versionDetails, "testMinecraft", username, offlineUuid(username), "0", baseDir);
}

void MainWindow::onManifestFetched(QList<VersionInfo> versions)
{
    m_versions = versions;
    m_logArea->append("> Versions loaded.");
    statusBar()->showMessage("idle");
    populateVersionList();


}

void MainWindow::populateVersionList(){
    m_versionSelection->clear();
    if(m_snapshotToggle->isChecked()){
        for(const VersionInfo &version : m_versions){
            m_versionSelection->addItem(version.id + " (" + version.type + ")");
        }
    } else {
        for(const VersionInfo &version : m_versions){
            if(version.type == "release"){
            m_versionSelection->addItem(version.id + " (" + version.type + ")");
            }
        }
    }
}


void MainWindow::onVersionJsonParsed(VersionDetails details){
    m_versionDetails = details;
    m_logArea->append("> Downloading libraries...");
    m_downloadManager = new DownloadManager(this);
    m_javaManager = new JavaManager(cacheDir, this);
    m_assetManager = new AssetManager(cacheDir, this);

    connect(m_downloadManager, &DownloadManager::finished, this, [this, details](){
        m_logArea->append("> Libraries done. Downloading assets...");
        connect(m_assetManager, &AssetManager::finished, this, [this, details](){
            m_logArea->append("> Assets done. Downloading Java...");
            connect(m_javaManager, &JavaManager::finished, this, [this](){
                m_logArea->append("> All downloads complete.");
                statusBar()->showMessage("idle");
            });
            m_javaManager->download(details.javaRuntimeName);
        });
        m_assetManager->download(details.assetIndexUrl, details.assetIndexId);
    });

    m_downloadManager->download(details, "testMinecraft");
}









































