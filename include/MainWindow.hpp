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
    static MainWindow* getInstance();  // Без parent
    MainWindow(const MainWindow&) = delete;
    MainWindow& operator=(const MainWindow&) = delete;

protected:
    void closeEvent(QCloseEvent* event) override;
    virtual ~MainWindow() = default;

private slots:
    void newFile();
    void openFile();
    void saveFile();
    void undo();
    void redo();
    void countCharacters();
    void toggleMode();
    void closeTab(int index);

private:
    explicit MainWindow();  // Только приватный
    static MainWindow* instance;

    QAction* saveAction;
    QTabWidget* tabs;
    QVector<std::shared_ptr<TextComponent>> editors;
    QVector<QString> filePaths;
    QVector<std::shared_ptr<TextReceiver>> receivers;
    QVector<std::vector<std::shared_ptr<Command>>> commandHistory;
    QVector<int> commandIndex;
    std::shared_ptr<DocumentSubject> subject;
    std::shared_ptr<FileFacade> fileFacade;
    std::shared_ptr<EditorContext> editorContext;
};

#endif // MAINWINDOW_HPP
