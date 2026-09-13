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
    m_confirmBtn = new QPushButton("Create", this);
    m_confirmBtn->setMaximumWidth(300);
    createLayout->addWidget(m_nameInput);
    createLayout->addWidget(m_versionCombo);
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
    if (name.isEmpty() || version.isEmpty()) return;
    m_instanceManager->createInstance(name, m_baseDir, version);
    m_nameInput->clear();
    populateInstanceList();
    m_stackedWidget->setCurrentIndex(m_cachedInstances.size());
}