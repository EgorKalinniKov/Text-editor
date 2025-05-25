#ifndef MAINWINDOW_HPP
#define MAINWINDOW_HPP

#include "DocumentAdapter.hpp"
#include "TextDecorator.hpp"
#include "EditCommand.hpp"
#include "DocumentObserver.hpp"
#include "TextIterator.hpp"
#include "FileFacade.hpp"
#include "FileProxy.hpp"
#include "EditorState.hpp"
#include <QTabWidget>
#include <QTextEdit>
#include <QVector>
#include <memory>
#include <QMainWindow>

class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    static MainWindow* getInstance(QWidget* parent = nullptr);

    MainWindow(const MainWindow&) = delete;
    MainWindow& operator=(const MainWindow&) = delete;

private slots:
    void openFile();
    void saveFile();
    void undo();
    void redo();
    void countCharacters();
    void toggleMode();

private:
    explicit MainWindow(QWidget* parent = nullptr);

    QTabWidget* tabs;
    QVector<std::shared_ptr<TextComponent>> editors;
    QVector<QString> filePaths;
    QVector<std::shared_ptr<TextReceiver>> receivers;
    QVector<std::vector<std::shared_ptr<Command>>> commandHistory;
    QVector<int> commandIndex;
    std::shared_ptr<DocumentSubject> subject;
    std::shared_ptr<FileFacade> fileFacade;
    std::shared_ptr<EditorContext> editorContext;

    static std::unique_ptr<MainWindow> instance;
};

#endif // MAINWINDOW_HPP
