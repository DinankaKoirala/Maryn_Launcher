#include "MainWindow.h"
#include "ui_MainWindow.h"
#include <QDebug>
#include <QDir>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    m_cacheDir = QDir::homePath() + "/.local/share/MarynLauncher";
    ui->setupUi(this);

    m_versionManifest = new VersionManifest(this);

    connect(m_versionManifest, &VersionManifest::finished,
            this, [this](QList<VersionInfo> versions) {
                qDebug() << "Got" << versions.size() << "versions";

                m_versionParser = new VersionJsonParser(m_cacheDir, this);

                connect(m_versionParser, &VersionJsonParser::finished,
                        this, [this](VersionDetails details) {
                            qDebug() << "libraries count:" << details.libraries.size();

                            m_downloadManager = new DownloadManager(this);

                            connect(m_downloadManager, &DownloadManager::progress,
                                    this, [](int current, int total) {
                                        qDebug() << "Downloading:" << current << "/" << total;
                                    });

                            connect(m_downloadManager, &DownloadManager::finished,
                                    this, []() {
                                        qDebug() << "All downloads complete!";
                                    });

                            m_downloadManager->download(details, "TestInstance");
                        });

                m_versionParser->fetch(versions.first().url);
            });

    m_versionManifest->fetch();
}

MainWindow::~MainWindow()
{
    delete ui;
}
