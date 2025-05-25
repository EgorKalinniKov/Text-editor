#ifndef SINGLETONWINDOW_HPP
#define SINGLETONWINDOW_HPP

#include <QMainWindow>
#include <memory>

class SingletonWindow : public QMainWindow {
    Q_OBJECT

public:
    static SingletonWindow* getInstance(QWidget* parent = nullptr);
    SingletonWindow(const SingletonWindow&) = delete;
    SingletonWindow& operator=(const SingletonWindow&) = delete;

private:
    SingletonWindow(QWidget* parent = nullptr);
    static std::unique_ptr<SingletonWindow> instance;
};

#endif // SINGLETONWINDOW_HPP