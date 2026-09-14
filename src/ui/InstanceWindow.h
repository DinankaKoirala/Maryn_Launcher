#pragma once 
#include <QMainWindow>
#include <QLabel>
#include <QPushButton>
#include <QComboBox>
#include <QCheckBox>
#include <QLineEdit>
#include <QStandardPaths>
#include <QListWidget>
#include <QStackedWidget>
#include "MainWindow.h"
#include "../core/instance/Headers/InstanceManager.h"
#include "../core/download/Headers/VersionManifest.h"
#include "../core/loader/Headers/LoaderManager.h"

class InstanceWindow : public QMainWindow
{
    Q_OBJECT

public:
    InstanceWindow(QWidget *parent = nullptr);
    ~InstanceWindow();

private slots:
    void populateInstanceList();
    void on_createNewBtn_clicked();
    void on_confirmClicked();
    void onManifestFetched(QList<VersionInfo> versions);
    void onLoaderChanged(int index);
    void onLoaderVersionsFetched(QStringList versions);

private:
    InstanceManager *m_instanceManager;
    LoaderManager *m_loaderManager;
    VersionManifest *m_versionManifest;
    QList<Instance> m_cachedInstances;
    QList<VersionInfo> m_versions;
    QString m_baseDir;
    QListWidget *m_instances;
    QStackedWidget *m_stackedWidget;
    QPushButton *m_createNewBtn;
    QPushButton *m_confirmBtn;
    QLineEdit *m_nameInput;
    QComboBox *m_versionCombo;
    QComboBox *m_loaderCombo;
    QComboBox *m_loaderVersionCombo;
};