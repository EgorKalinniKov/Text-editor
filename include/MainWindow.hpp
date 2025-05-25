#ifndef MAINWINDOW_HPP
#define MAINWINDOW_HPP

#include "SingletonWindow.hpp"
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

class MainWindow : public SingletonWindow {
    Q_OBJECT

public:
    MainWindow(QWidget* parent = nullptr);

private slots:
    void openFile();
    void saveFile();
    void undo();
    void redo();
    void countCharacters();
    void toggleMode();

private:
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