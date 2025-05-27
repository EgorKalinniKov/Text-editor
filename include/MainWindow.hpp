#pragma once

#include <QMainWindow>
#include <QTabWidget>
#include <QVector>
#include <memory>
#include <mutex>
#include "TextDecorator.hpp"
#include "EditCommand.hpp"
#include "DocumentObserver.hpp"
#include "TextIterator.hpp"
#include "FileFacade.hpp"
#include "EditorState.hpp"

class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    static MainWindow* getInstance();
    virtual ~MainWindow() = default;
    
    // Запрет копирования и присваивания
    MainWindow(const MainWindow&) = delete;
    MainWindow& operator=(const MainWindow&) = delete;

private:
    static MainWindow* instance;
    static std::mutex mutex;

    MainWindow();  // Приватный конструктор для синглтона
    void initializeUI();
    void initializeConnections();
    void setupMenus();
    void createFileMenu();
    void createEditMenu();
    void initializeComponents();
    void updateWindowTitle();
    std::shared_ptr<TextComponent> createTextComponent(QTextEdit* textEdit);
    void openFileAtPath(const QString& filePath);
    void saveFileToPath(const QString& path);
    bool hasUnsavedChanges(int index);
    bool confirmClose();
    void removeTab(int index);
    void cleanup();

    QTabWidget* tabs;
    QAction* saveAction;
    int currentIndex;

    QVector<std::shared_ptr<TextComponent>> editors;
    QVector<QString> filePaths;
    QVector<std::shared_ptr<TextReceiver>> receivers;
    QVector<std::vector<std::shared_ptr<EditCommand>>> commandHistory;
    QVector<int> commandIndex;

    std::shared_ptr<DocumentSubject> subject;
    std::shared_ptr<FileFacade> fileFacade;
    std::shared_ptr<EditorContext> editorContext;

private slots:
    void newFile();
    void openFile();
    void saveFile();
    void closeTab(int index);
    void onTabChanged(int index);
    void undo();
    void redo();
    void countCharacters();
    void toggleMode();

protected:
    void closeEvent(QCloseEvent* event) override;
};
