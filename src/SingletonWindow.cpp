#include "SingletonWindow.hpp"
#include "MainWindow.hpp"

std::unique_ptr<SingletonWindow> SingletonWindow::instance = nullptr;

SingletonWindow::SingletonWindow(QWidget* parent) : QMainWindow(parent) {}

SingletonWindow* SingletonWindow::getInstance(QWidget* parent) {
    if (!instance) {
        instance = std::unique_ptr<SingletonWindow>(new MainWindow(parent)); // Create MainWindow instance
    }
    return instance.get();
}