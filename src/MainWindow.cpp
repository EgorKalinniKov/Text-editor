#include "MainWindow.hpp"
#include <QMenuBar>
#include <QFileDialog>
#include <QMessageBox>
#include <QCloseEvent>
#include <QFileInfo>

MainWindow* MainWindow::instance = nullptr;
std::mutex MainWindow::mutex;

MainWindow* MainWindow::getInstance() {
    std::lock_guard<std::mutex> lock(mutex);
    if (!instance) {
        instance = new MainWindow();
    }
    return instance;
}

MainWindow::MainWindow() : currentIndex(-1) {
    initializeUI();
    initializeConnections();
    setupMenus();
    initializeComponents();
}

void MainWindow::initializeUI() {
    resize(1200, 900);
    tabs = new QTabWidget(this);
    tabs->setTabsClosable(true);
    setCentralWidget(tabs);
}

void MainWindow::initializeConnections() {
    connect(tabs, &QTabWidget::tabCloseRequested, this, &MainWindow::closeTab);
    connect(tabs, &QTabWidget::currentChanged, this, &MainWindow::onTabChanged);
}

void MainWindow::setupMenus() {
    createFileMenu();
    createEditMenu();
}

void MainWindow::createFileMenu() {
    QMenu* fileMenu = menuBar()->addMenu(tr("File"));
    fileMenu->addAction(tr("New"), this, &MainWindow::newFile, QKeySequence::New);
    fileMenu->addAction(tr("Open"), this, &MainWindow::openFile, QKeySequence::Open);
    saveAction = fileMenu->addAction(tr("Save"), this, &MainWindow::saveFile, QKeySequence::Save);
}

void MainWindow::createEditMenu() {
    QMenu* editMenu = menuBar()->addMenu(tr("Edit"));
    editMenu->addAction(tr("Undo"), this, &MainWindow::undo, QKeySequence::Undo);
    editMenu->addAction(tr("Redo"), this, &MainWindow::redo, QKeySequence::Redo);
    editMenu->addAction(tr("Count Characters"), this, &MainWindow::countCharacters);
    editMenu->addAction(tr("Toggle Mode"), this, &MainWindow::toggleMode);
}

void MainWindow::initializeComponents() {
    subject = std::make_shared<DocumentSubject>();
    fileFacade = std::make_shared<FileFacade>();
    editorContext = std::make_shared<EditorContext>();
}

void MainWindow::onTabChanged(int index) {
    currentIndex = index;
    updateWindowTitle();
}

void MainWindow::updateWindowTitle() {
    if (currentIndex >= 0 && currentIndex < filePaths.size()) {
        QString fileName = QFileInfo(filePaths[currentIndex]).fileName();
        setWindowTitle(fileName.isEmpty() ? tr("Untitled") : fileName);
    } else {
        setWindowTitle(tr("Text Editor"));
    }
}

std::shared_ptr<TextComponent> MainWindow::createTextComponent(QTextEdit* textEdit) {
    auto simpleText = std::make_shared<SimpleTextEdit>(textEdit);
    auto spellChecked = std::make_shared<SpellCheckDecorator>(simpleText);
    return std::make_shared<GrammarCheckDecorator>(spellChecked);
}

void MainWindow::newFile() {
    QTextEdit* textEdit = new QTextEdit();
    auto editor = createTextComponent(textEdit);
    
    editors.push_back(editor);
    receivers.push_back(std::make_shared<TextReceiver>());
    filePaths.push_back("");
    commandHistory.push_back(std::vector<std::shared_ptr<EditCommand>>());
    commandIndex.push_back(-1);

    int index = tabs->addTab(textEdit, tr("Untitled"));
    tabs->setCurrentIndex(index);
}

void MainWindow::closeTab(int index) {
    if (index < 0 || index >= tabs->count()) return;

    if (hasUnsavedChanges(index)) {
        if (!confirmClose()) return;
    }

    removeTab(index);
}

bool MainWindow::hasUnsavedChanges(int index) {
    // Implement change detection logic here
    return false;
}

bool MainWindow::confirmClose() {
    QMessageBox::StandardButton reply = QMessageBox::question(
        this, tr("Unsaved Changes"),
        tr("Do you want to save your changes?"),
        QMessageBox::Yes | QMessageBox::No | QMessageBox::Cancel
    );

    if (reply == QMessageBox::Cancel) return false;
    if (reply == QMessageBox::Yes) {
        saveFile();
    }
    return true;
}

void MainWindow::removeTab(int index) {
    tabs->removeTab(index);
    editors.removeAt(index);
    filePaths.removeAt(index);
    receivers.removeAt(index);
    commandHistory.removeAt(index);
    commandIndex.removeAt(index);
}

void MainWindow::closeEvent(QCloseEvent* event) {
    bool canClose = true;
    for (int i = 0; i < tabs->count() && canClose; ++i) {
        if (hasUnsavedChanges(i)) {
            tabs->setCurrentIndex(i);
            canClose = confirmClose();
        }
    }

    if (canClose) {
        cleanup();
        event->accept();
    } else {
        event->ignore();
    }
}

void MainWindow::cleanup() {
    tabs->clear();
    editors.clear();
    filePaths.clear();
    receivers.clear();
    commandHistory.clear();
    commandIndex.clear();
}

void MainWindow::openFile() {
    QString filePath = QFileDialog::getOpenFileName(
        this, tr("Open File"),
        QString(),
        tr("Text Files (*.txt);;All Files (*.*)")
    );
    
    if (filePath.isEmpty()) return;

    openFileAtPath(filePath);
}

void MainWindow::openFileAtPath(const QString& filePath) {
    QString format = QFileInfo(filePath).suffix();
    QString content = fileFacade->loadFile(filePath, "." + format);

    QTextEdit* textEdit = new QTextEdit();
    auto editor = createTextComponent(textEdit);
    textEdit->setText(content);

    editors.push_back(editor);
    filePaths.push_back(filePath);
    receivers.push_back(std::make_shared<TextReceiver>(content));
    commandHistory.push_back({});
    commandIndex.push_back(-1);

    int index = tabs->addTab(textEdit, QFileInfo(filePath).fileName());
    tabs->setCurrentIndex(index);
}

void MainWindow::saveFile() {
    if (currentIndex < 0) return;

    QString& path = filePaths[currentIndex];
    if (path.isEmpty()) {
        path = QFileDialog::getSaveFileName(
            this, tr("Save File"),
            QString(),
            tr("Text Files (*.txt);;All Files (*.*)")
        );
        if (path.isEmpty()) return;
    }

    saveFileToPath(path);
}

void MainWindow::saveFileToPath(const QString& path) {
    QTextEdit* editorWidget = qobject_cast<QTextEdit*>(tabs->currentWidget());
    if (!editorWidget) return;

    QString content = editorWidget->toPlainText();

    if (fileFacade->saveFile(path, content)) {
        tabs->setTabText(currentIndex, QFileInfo(path).fileName());
        updateWindowTitle();
    } else {
        QMessageBox::warning(this, tr("Error"), tr("Failed to save file."));
    }
}

void MainWindow::undo() {
    if (currentIndex < 0 || commandIndex[currentIndex] < 0) return;

    auto& commands = commandHistory[currentIndex];
    auto& index = commandIndex[currentIndex];
    
    commands[index]->undo();
    index--;
}

void MainWindow::redo() {
    if (currentIndex < 0) return;
    
    auto& commands = commandHistory[currentIndex];
    auto& index = commandIndex[currentIndex];
    
    if (index >= static_cast<int>(commands.size()) - 1) return;

    index++;
    commands[index]->execute();
}

void MainWindow::countCharacters() {
    if (currentIndex < 0) return;

    QString text = editors[currentIndex]->getText();
    auto aggregate = std::make_shared<ConcreteTextAggregate>(text);
    auto it = aggregate->createIterator();

    int count = 0;
    while (it->hasNext()) {
        it->next();
        count++;
    }

    QMessageBox::information(
        this, 
        tr("Characters"), 
        tr("Total characters: %1").arg(count)
    );
}

void MainWindow::toggleMode() {
    if (editorContext->getStatus().contains("read-only")) {
        editorContext->setState(std::make_shared<EditableState>());
    } else {
        editorContext->setState(std::make_shared<ReadOnlyState>());
    }

    editorContext->requestEdit("Toggle");
    QMessageBox::information(this, tr("Mode"), editorContext->getStatus());
}
