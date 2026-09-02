#include <QApplication>
#include "MainWindow.h"
#include "src/ui/MainWindow.h"

int main(int argc, char *argv[]){
    QApplication app(argc, argv);

    MainWindow window;
    window.show();

    app.setStyleSheet(R"(
    QWidget {
        background-color: #1a1a1a;
        color: #d4d4d4;
        font-family: 'Segoe UI';
        font-size: 13px;
    }
    QComboBox {
        background-color: #2a2a2a;
        border: 1px solid #3a3a3a;
        border-radius: 4px;
        padding: 6px 10px;
        color: #d4d4d4;
    }
    QComboBox:hover { border-color: #555; }
    QComboBox QAbstractItemView {
        background-color: #2a2a2a;
        border: 1px solid #3a3a3a;
        selection-background-color: #1a3a2a;
        selection-color: #4ec97a;
    }
    QPushButton {
        background-color: #2a2a2a;
        border: 1px solid #3a3a3a;
        border-radius: 4px;
        padding: 8px 16px;
        color: #aaa;
    }
    QPushButton:hover { background-color: #333; color: #ccc; }
    QPushButton#launchBtn {
        background-color: #1a3a2a;
        border-color: #2a6a45;
        color: #4ec97a;
    }
    QPushButton#launchBtn:hover { background-color: #1f4530; }
    QTextEdit {
        background-color: #111;
        border: 1px solid #2a2a2a;
        border-radius: 4px;
        font-family: 'Consolas';
        font-size: 12px;
        color: #555;
    }
    QStatusBar {
        background-color: #111;
        color: #555;
        font-family: 'Consolas';
        font-size: 11px;
        border-top: 1px solid #2a2a2a;
}
    )");

    return app.exec();
}
