#include "MainWindow.hpp"
#include <QMenuBar>
#include <QFileDialog>
#include <QMessageBox>
#include <QCloseEvent>
#include <QFileInfo>
#include <QTextCharFormat>
#include <QIcon>
#include <QStatusBar>
#include <QLabel>
#include <QRegularExpression>

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
    setupToolBar();
    setupStatusBar();
}

void MainWindow::setupToolBar() {
    toolBar = addToolBar(tr("Tools"));
    toolBar->setMovable(false);

    // File operations
    newAction = toolBar->addAction(QIcon::fromTheme("document-new"), tr("New"));
    connect(newAction, &QAction::triggered, this, &MainWindow::newFile);

    openAction = toolBar->addAction(QIcon::fromTheme("document-open"), tr("Open"));
    connect(openAction, &QAction::triggered, this, &MainWindow::openFile);

    saveAction = toolBar->addAction(QIcon::fromTheme("document-save"), tr("Save"));
    connect(saveAction, &QAction::triggered, this, &MainWindow::saveFile);

    toolBar->addSeparator();

    // Edit operations
    undoAction = toolBar->addAction(QIcon::fromTheme("edit-undo"), tr("Undo"));
    connect(undoAction, &QAction::triggered, this, &MainWindow::undo);

    redoAction = toolBar->addAction(QIcon::fromTheme("edit-redo"), tr("Redo"));
    connect(redoAction, &QAction::triggered, this, &MainWindow::redo);

    toolBar->addSeparator();

    // Text formatting
    boldAction = toolBar->addAction(QIcon::fromTheme("format-text-bold"), tr("Bold"));
    connect(boldAction, &QAction::triggered, this, &MainWindow::toggleBold);

    italicAction = toolBar->addAction(QIcon::fromTheme("format-text-italic"), tr("Italic"));
    connect(italicAction, &QAction::triggered, this, &MainWindow::toggleItalic);

    colorAction = toolBar->addAction(QIcon::fromTheme("format-text-color"), tr("Color"));
    connect(colorAction, &QAction::triggered, this, &MainWindow::chooseColor);

    updateActions();
}

void MainWindow::setupStatusBar() {
    statusBar = new QStatusBar(this);
    setStatusBar(statusBar);

    charCountLabel = new QLabel(tr("Characters: 0"));
    wordCountLabel = new QLabel(tr("Words: 0"));

    statusBar->addPermanentWidget(charCountLabel);
    statusBar->addPermanentWidget(wordCountLabel);
}

void MainWindow::initializeConnections() {
    connect(tabs, &QTabWidget::tabCloseRequested, this, &MainWindow::closeTab);
    connect(tabs, &QTabWidget::currentChanged, this, [this](int index) {
        onTabChanged(index);
        updateActions();
        updateTextStatistics();
    });
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
    updateTextStatistics();
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
    auto italicText = std::make_shared<ItalicDecorator>(simpleText);
    auto colorText = std::make_shared<ColorDecorator>(italicText, Qt::blue);
    return std::make_shared<BoldDecorator>(colorText);
}

void MainWindow::newFile() {
    QTextEdit* textEdit = new QTextEdit();
    connect(textEdit, &QTextEdit::textChanged, this, &MainWindow::onTextChanged);
    auto editor = createTextComponent(textEdit);
    
    editors.push_back(editor);
    receivers.push_back(std::make_shared<TextReceiver>());
    filePaths.push_back("");
    commandHistory.push_back(std::vector<std::shared_ptr<ICommand>>());
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
    textEdit->setHtml(content);
    auto editor = createTextComponent(textEdit);

    editors.push_back(editor);
    filePaths.push_back(filePath);
    receivers.push_back(std::make_shared<TextReceiver>());
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

    QString content;
    if (auto editor = editors[currentIndex]) {
        content = editor->getFormattedText();
    } else {
        content = editorWidget->toHtml();
    }

    if (fileFacade->saveFile(path, content)) {
        tabs->setTabText(currentIndex, QFileInfo(path).fileName());
        filePaths[currentIndex] = path;
        updateWindowTitle();
    } else {
        QMessageBox::warning(this, tr("Error"), tr("Failed to save file."));
    }
}

void MainWindow::undo() {
    if (QTextEdit* editor = getCurrentEditor()) {
        editor->undo();
    }
}

void MainWindow::redo() {
    if (QTextEdit* editor = getCurrentEditor()) {
        editor->redo();
    }
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

QTextEdit* MainWindow::getCurrentEditor() const {
    if (currentIndex >= 0) {
        return qobject_cast<QTextEdit*>(tabs->widget(currentIndex));
    }
    return nullptr;
}

void MainWindow::updateActions() {
    bool hasEditor = getCurrentEditor() != nullptr;
    undoAction->setEnabled(hasEditor);
    redoAction->setEnabled(hasEditor);
    boldAction->setEnabled(hasEditor);
    italicAction->setEnabled(hasEditor);
    colorAction->setEnabled(hasEditor);
}

void MainWindow::applyTextFormat(const std::function<void(QTextEdit*)>& formatter) {
    QTextEdit* editor = getCurrentEditor();
    if (editor) {
        formatter(editor);
    }
}

void MainWindow::toggleBold() {
    applyTextFormat([](QTextEdit* editor) {
        QTextCharFormat format = editor->currentCharFormat();
        format.setFontWeight(format.fontWeight() == QFont::Bold ? QFont::Normal : QFont::Bold);
        editor->mergeCurrentCharFormat(format);
    });
}

void MainWindow::toggleItalic() {
    applyTextFormat([](QTextEdit* editor) {
        QTextCharFormat format = editor->currentCharFormat();
        format.setFontItalic(!format.fontItalic());
        editor->mergeCurrentCharFormat(format);
    });
}

void MainWindow::chooseColor() {
    QTextEdit* editor = getCurrentEditor();
    if (!editor) return;

    QColor color = QColorDialog::getColor(editor->textColor(), this);
    if (color.isValid()) {
        applyTextFormat([color](QTextEdit* editor) {
            QTextCharFormat format;
            format.setForeground(color);
            editor->mergeCurrentCharFormat(format);
        });
    }
}

void MainWindow::onTextChanged() {
    updateTextStatistics();
}

void MainWindow::updateTextStatistics() {
    QTextEdit* editor = getCurrentEditor();
    if (!editor) {
        charCountLabel->setText(tr("Characters: 0"));
        wordCountLabel->setText(tr("Words: 0"));
        return;
    }

    QString text = editor->toPlainText();
    auto aggregate = std::make_shared<ConcreteTextAggregate>(text);
    auto iterator = aggregate->createIterator();

    charCountLabel->setText(tr("Characters: %1").arg(iterator->getCharCount()));
    wordCountLabel->setText(tr("Words: %1").arg(iterator->getWordCount()));
}
