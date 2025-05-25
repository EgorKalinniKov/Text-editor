#include "MainWindow.hpp"
#include <QApplication>
#include <QTextCodec>

int main(int argc, char* argv[]) {

    #if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
        QTextCodec::setCodecForLocale(QTextCodec::codecForName("UTF-8"));
    #endif

    QApplication app(argc, argv);
    MainWindow* window = MainWindow::getInstance();
    window->show();
    return app.exec();
}
