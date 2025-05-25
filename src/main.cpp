#include "MainWindow.hpp"
#include <QApplication>

int main(int argc, char* argv[]) {
    QApplication app(argc, argv);
    MainWindow* window = dynamic_cast<MainWindow*>(MainWindow::getInstance()); // Safe cast
    if (window) {
        window->show();
        return app.exec();
    }
    return -1; // Handle failure
}