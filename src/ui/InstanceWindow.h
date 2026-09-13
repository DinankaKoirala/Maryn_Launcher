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

private:
    InstanceManager *m_instanceManager;
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
};