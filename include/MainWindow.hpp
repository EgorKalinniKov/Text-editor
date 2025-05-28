#pragma once

#include <QMainWindow>
#include <QTabWidget>
#include <QVector>
#include <QToolBar>
#include <QAction>
#include <QColorDialog>
#include <QStatusBar>
#include <QLabel>
#include <memory>
#include <mutex>
#include "TextDecorator.hpp"
#include "Command.hpp"
#include "DocumentObserver.hpp"
#include "TextIterator.hpp"
#include "EditorState.hpp"
#include "DocumentAdapter.hpp"

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
    void setupToolBar();
    void setupStatusBar();
    void createFileMenu();
    void initializeComponents();
    void updateWindowTitle();
    void updateTextStatistics();
    void updateDocumentState();
    std::shared_ptr<TextComponent> createTextComponent(QTextEdit* textEdit);
    void openFileAtPath(const QString& filePath);
    void saveFileToPath(const QString& path);
    bool hasUnsavedChanges(int index);
    bool confirmClose();
    void removeTab(int index);
    void cleanup();
    void applyTextFormat(const std::function<void(QTextEdit*)>& formatter);
    QTextEdit* getCurrentEditor() const;

    QTabWidget* tabs;
    QToolBar* toolBar;
    QStatusBar* statusBar;
    QLabel* charCountLabel;
    QLabel* wordCountLabel;
    QLabel* stateLabel;
    QAction* saveAction;
    QAction* newAction;
    QAction* openAction;
    QAction* undoAction;
    QAction* redoAction;
    QAction* boldAction;
    QAction* italicAction;
    QAction* colorAction;
    int currentIndex;

    QVector<std::shared_ptr<TextComponent>> editors;
    QVector<QString> filePaths;
    QVector<std::shared_ptr<ITextReceiver>> receivers;
    QVector<std::vector<std::shared_ptr<ICommand>>> commandHistory;
    QVector<int> commandIndex;

    std::shared_ptr<DocumentSubject> subject;
    std::shared_ptr<EditorContext> editorContext;

private slots:
    void newFile();
    void openFile();
    void saveFile();
    void closeTab(int index);
    void onTabChanged(int index);
    void undo();
    void redo();
    void toggleBold();
    void toggleItalic();
    void chooseColor();
    void updateActions();
    void onTextChanged();

protected:
    void closeEvent(QCloseEvent* event) override;
};
