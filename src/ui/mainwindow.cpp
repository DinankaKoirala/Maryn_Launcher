#include "Mainwindow.h"
#include "ui_Mainwindow.h"
#include <QDebug>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    m_versionManifest = new VersionManifest(this);

    connect(m_versionManifest, &VersionManifest::finished,
            this, [](QList<VersionInfo> versions) {
                qDebug() << "Got" << versions.size() << "versions";
                qDebug() << "First version:" << versions.first().id;
            });

    connect(m_versionManifest, &VersionManifest::errorOccurred,
            this, [](QString error) {
                qDebug() << "Error:" << error;
            });

    m_versionManifest->fetch();

    connect(m_versionManifest, &VersionManifest::finished,
        this, [this](QList<VersionInfo> versions) {
            qDebug() << "Got" << versions.size() << "versions";
            qDebug() << "First version:" << versions.first().id;

            // test VersionJsonParser with the first version
            m_versionParser = new VersionJsonParser(this);

            connect(m_versionParser, &VersionJsonParser::finished,
                    this, [](VersionDetails details) {
                        qDebug() << "mainClass:" << details.mainClass;
                        qDebug() << "javaVersion:" << details.javaVersion;
                        qDebug() << "clientUrl:" << details.clientUrl;
                        qDebug() << "libraries count:" << details.libraries.size();
                    });

            connect(m_versionParser, &VersionJsonParser::errorOccurred,
                    this, [](QString error) {
                        qDebug() << "Parser error:" << error;
                    });

            m_versionParser->fetch(versions.first().url);
        });
}

MainWindow::~MainWindow()
{
    delete ui;
}