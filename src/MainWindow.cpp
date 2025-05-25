#include "MainWindow.hpp"
#include <QMenuBar>
#include <QFileDialog>
#include <QMessageBox>
#include <QFileInfo>

std::unique_ptr<MainWindow> MainWindow::instance = nullptr;

MainWindow* MainWindow::getInstance(QWidget* parent) {
    if (!instance) {
        instance.reset(new MainWindow(parent));
    }
    return instance.get();
}

MainWindow::MainWindow(QWidget* parent) : QMainWindow(parent) {
    tabs = new QTabWidget(this);
    setCentralWidget(tabs);

    // Setup menu
    QMenu* fileMenu = menuBar()->addMenu("File");
    fileMenu->addAction("Open", this, &MainWindow::openFile);
    fileMenu->addAction("Save", this, &MainWindow::saveFile);

    QMenu* editMenu = menuBar()->addMenu("Edit");
    editMenu->addAction("Undo", this, &MainWindow::undo);
    editMenu->addAction("Redo", this, &MainWindow::redo);
    editMenu->addAction("Count Characters", this, &MainWindow::countCharacters);
    editMenu->addAction("Toggle Mode", this, &MainWindow::toggleMode);

    subject = std::make_shared<DocumentSubject>();
    fileFacade = std::make_shared<FileFacade>();
    editorContext = std::make_shared<EditorContext>();
}

void MainWindow::openFile() {
    QString filePath = QFileDialog::getOpenFileName(this, "Open File");
    if (filePath.isEmpty()) return;

    QString format = QFileInfo(filePath).suffix();
    QString content = fileFacade->loadFile(filePath, "." + format);

    QTextEdit* textEdit = new QTextEdit();
    auto simpleText = std::make_shared<SimpleTextEdit>(textEdit);
    auto decoratedText = std::make_shared<SpellCheckDecorator>(simpleText);
    textEdit->setText(content);

    editors.push_back(decoratedText);
    filePaths.push_back(filePath);
    receivers.push_back(std::make_shared<TextReceiver>(content));
    commandHistory.push_back({});
    commandIndex.push_back(-1);

    tabs->addTab(textEdit, QFileInfo(filePath).fileName());
}

void MainWindow::saveFile() {
    int currentTab = tabs->currentIndex();
    if (currentTab == -1) return;

    QString content = editors[currentTab]->getText();
    fileFacade->saveFile(filePaths[currentTab], content, QFileInfo(filePaths[currentTab]).suffix());
}

void MainWindow::undo() {
    int currentTab = tabs->currentIndex();
    if (currentTab == -1 || commandIndex[currentTab] < 0) return;

    commandHistory[currentTab][commandIndex[currentTab]]->undo();
    commandIndex[currentTab]--;
}

void MainWindow::redo() {
    int currentTab = tabs->currentIndex();
    if (currentTab == -1 || commandIndex[currentTab] >= (int)commandHistory[currentTab].size() - 1) return;

    commandIndex[currentTab]++;
    commandHistory[currentTab][commandIndex[currentTab]]->execute();
}

void MainWindow::countCharacters() {
    int currentTab = tabs->currentIndex();
    if (currentTab == -1) return;

    QString text = editors[currentTab]->getText();
    auto aggregate = std::make_shared<ConcreteTextAggregate>(text);
    auto iterator = aggregate->createIterator();

    int count = 0;
    while (iterator->hasNext()) {
        iterator->next();
        count++;
    }

    QMessageBox::information(this, "Character Count", "Total characters: " + QString::number(count));
}

void MainWindow::toggleMode() {
    if (editorContext->getStatus().contains("read-only")) {
        editorContext->setState(std::make_shared<EditableState>());
    } else {
        editorContext->setState(std::make_shared<ReadOnlyState>());
    }
    editorContext->requestEdit("Mode toggle");
    QMessageBox::information(this, "Mode", editorContext->getStatus());
}
