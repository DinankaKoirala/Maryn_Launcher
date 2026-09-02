#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "VersionManifest.h"
#include "VersionJsonParser.h"
#include "DownloadManager.h"

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow() override;

private:
    Ui::MainWindow *ui;
    VersionManifest *m_versionManifest;
    VersionJsonParser *m_versionParser;
    DownloadManager *m_downloadManager;
    QString m_cacheDir;
};
#endif // MAINWINDOW_H
