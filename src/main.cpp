#include "MainWindow.hpp"
#include <QApplication>

int main(int argc, char* argv[]) {
    QApplication app(argc, argv);
    MainWindow* window = MainWindow::getInstance();
    window->show();
    return app.exec();
}
