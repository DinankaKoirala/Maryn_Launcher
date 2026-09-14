#include "InstanceWindow.h"
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QStatusBar>
#include <QSplitter>
#include <QDir>
#include <QLabel>

InstanceWindow::InstanceWindow(QWidget *parent)
    : QMainWindow(parent)
{
    setWindowTitle("Maryn Launcher");
    setMinimumSize(720, 480);

    m_baseDir = QStandardPaths::writableLocation(QStandardPaths::AppDataLocation);
    m_instanceManager = new InstanceManager();
    m_loaderManager = new LoaderManager(this);

    QWidget *centralWidget = new QWidget(this);
    setCentralWidget(centralWidget);
    QHBoxLayout *mainLayout = new QHBoxLayout(centralWidget);
    mainLayout->setContentsMargins(10, 10, 10, 10);

    QSplitter *mainSplitter = new QSplitter(Qt::Horizontal, this);
    mainLayout->addWidget(mainSplitter);

    // Left panel
    QWidget *leftPanel = new QWidget(this);
    QVBoxLayout *leftLayout = new QVBoxLayout(leftPanel);
    leftLayout->setContentsMargins(0, 0, 0, 0);
    m_instances = new QListWidget(this);
    m_createNewBtn = new QPushButton("+", this);
    leftLayout->addWidget(m_instances);
    leftLayout->addWidget(m_createNewBtn);
    mainSplitter->addWidget(leftPanel);

    // Right panel
    m_stackedWidget = new QStackedWidget(this);
    mainSplitter->addWidget(m_stackedWidget);

    // Page 0 — create new instance form
    QWidget *createPage = new QWidget(this);
    QVBoxLayout *createLayout = new QVBoxLayout(createPage);
    createLayout->setAlignment(Qt::AlignCenter);

    m_nameInput = new QLineEdit(this);
    m_nameInput->setPlaceholderText("Instance name");
    m_nameInput->setMaximumWidth(300);

    m_versionCombo = new QComboBox(this);
    m_versionCombo->setMaximumWidth(300);

    m_loaderCombo = new QComboBox(this);
    m_loaderCombo->setMaximumWidth(300);
    m_loaderCombo->addItem("Vanilla", static_cast<int>(LoaderType::Vanilla));
    m_loaderCombo->addItem("Fabric",  static_cast<int>(LoaderType::Fabric));
    m_loaderCombo->addItem("Quilt",   static_cast<int>(LoaderType::Quilt));

    m_loaderVersionCombo = new QComboBox(this);
    m_loaderVersionCombo->setMaximumWidth(300);
    m_loaderVersionCombo->setEnabled(false); // disabled until a non-vanilla loader is picked

    m_confirmBtn = new QPushButton("Create", this);
    m_confirmBtn->setMaximumWidth(300);

    createLayout->addWidget(m_nameInput);
    createLayout->addWidget(m_versionCombo);
    createLayout->addWidget(m_loaderCombo);
    createLayout->addWidget(m_loaderVersionCombo);
    createLayout->addWidget(m_confirmBtn);
    m_stackedWidget->addWidget(createPage);

    QList<int> splitterSizes;
    splitterSizes << 200 << 520;
    mainSplitter->setSizes(splitterSizes);

    connect(m_createNewBtn, &QPushButton::clicked, this, &InstanceWindow::on_createNewBtn_clicked);
    connect(m_instances, &QListWidget::currentRowChanged, this, [this](int row) {
        m_stackedWidget->setCurrentIndex(row + 1);
    });
    connect(m_confirmBtn, &QPushButton::clicked, this, &InstanceWindow::on_confirmClicked);
    connect(m_loaderCombo, &QComboBox::currentIndexChanged, this, &InstanceWindow::onLoaderChanged);
    connect(m_versionCombo, &QComboBox::currentIndexChanged, this, [this](int) {
        onLoaderChanged(m_loaderCombo->currentIndex());
    });
    connect(m_loaderManager, &LoaderManager::loaderVersionsFetched, this, &InstanceWindow::onLoaderVersionsFetched);
    connect(m_loaderManager, &LoaderManager::errorOccurred, this, [this](QString msg) {
        qDebug() << "> LoaderManager error:" << msg;
        statusBar()->showMessage("Failed to fetch loader versions");
    });

    // Fetch version manifest
    m_versionManifest = new VersionManifest(this);
    connect(m_versionManifest, &VersionManifest::finished, this, &InstanceWindow::onManifestFetched);
    connect(m_versionManifest, &VersionManifest::errorOccurred, this, [this]() {
        qDebug() << "> Error: failed to fetch version manifest.";
        statusBar()->showMessage("error");
    });
    m_versionManifest->fetch();
}

InstanceWindow::~InstanceWindow() {}

void InstanceWindow::onManifestFetched(QList<VersionInfo> versions) {
    m_versions = versions;
    m_versionCombo->clear();
    for (const VersionInfo &v : versions) {
        if (v.type == "release") {
            m_versionCombo->addItem(v.id);
        }
    }
    statusBar()->showMessage("Ready");
    populateInstanceList();
}

void InstanceWindow::onLoaderChanged(int index) {
    LoaderType loader = static_cast<LoaderType>(m_loaderCombo->itemData(index).toInt());
    QString mcVersion = m_versionCombo->currentText();

    if (loader == LoaderType::Vanilla) {
        m_loaderVersionCombo->clear();
        m_loaderVersionCombo->setEnabled(false);
        return;
    }

    m_loaderVersionCombo->clear();
    m_loaderVersionCombo->setEnabled(false);
    statusBar()->showMessage("Fetching loader versions...");
    m_loaderManager->fetchLoaderVersions(mcVersion, loader);
}

void InstanceWindow::onLoaderVersionsFetched(QStringList versions) {
    m_loaderVersionCombo->clear();
    m_loaderVersionCombo->addItems(versions);
    m_loaderVersionCombo->setEnabled(true);
    statusBar()->showMessage("Ready");
}

void InstanceWindow::on_createNewBtn_clicked() {
    m_stackedWidget->setCurrentIndex(0);
}

void InstanceWindow::populateInstanceList() {
    m_instances->clear();
    m_cachedInstances = m_instanceManager->fetchInstancesList(m_baseDir);
    for (const Instance &instance : m_cachedInstances) {
        m_instances->addItem(instance.name);
        MainWindow *instancePage = new MainWindow(instance.name, instance.version, m_versions, this);
        m_stackedWidget->addWidget(instancePage);
    }
}

void InstanceWindow::on_confirmClicked() {
    QString name = m_nameInput->text().trimmed();
    QString version = m_versionCombo->currentText();
    LoaderType loader = static_cast<LoaderType>(m_loaderCombo->currentData().toInt());
    QString loaderStr = m_loaderCombo->currentText().toLower();
    QString loaderVersion = (loader == LoaderType::Vanilla) ? "" : m_loaderVersionCombo->currentText();

    if (name.isEmpty() || version.isEmpty()) return;
    if (loader != LoaderType::Vanilla && loaderVersion.isEmpty()) return;

    m_instanceManager->createInstance(name, m_baseDir, version, loaderStr, loaderVersion);
    m_nameInput->clear();
    populateInstanceList();
    m_stackedWidget->setCurrentIndex(m_cachedInstances.size());
}