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
}

MainWindow::~MainWindow()
{
    delete ui;
}