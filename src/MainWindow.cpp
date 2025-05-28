#include "MainWindow.hpp"
#include "DocumentAdapter.hpp"
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

    // Добавляем метки для статистики и состояния
    stateLabel = new QLabel();
    charCountLabel = new QLabel(tr("Characters: 0"));
    wordCountLabel = new QLabel(tr("Words: 0"));

    // Добавляем разделители для лучшего визуального разделения
    statusBar->addPermanentWidget(new QLabel(" | "));
    statusBar->addPermanentWidget(stateLabel);
    statusBar->addPermanentWidget(new QLabel(" | "));
    statusBar->addPermanentWidget(charCountLabel);
    statusBar->addPermanentWidget(new QLabel(" | "));
    statusBar->addPermanentWidget(wordCountLabel);

    // Устанавливаем начальное состояние
    updateDocumentState();
}

void MainWindow::updateDocumentState() {
    if (editorContext) {
        QString state = editorContext->getStatus();
        stateLabel->setText(state);
        
        // Изменяем цвет текста в зависимости от состояния
        if (state.contains("Modified")) {
            stateLabel->setStyleSheet("QLabel { color: #FF6B6B; }"); // Красный для измененного
        } else if (state.contains("Saved")) {
            stateLabel->setStyleSheet("QLabel { color: #4CAF50; }"); // Зеленый для сохраненного
        } else {
            stateLabel->setStyleSheet(""); // Сброс стиля
        }
    }
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
}

void MainWindow::createFileMenu() {
    QMenu* fileMenu = menuBar()->addMenu(tr("File"));
    fileMenu->addAction(tr("New"), this, &MainWindow::newFile, QKeySequence::New);
    fileMenu->addAction(tr("Open"), this, &MainWindow::openFile, QKeySequence::Open);
    saveAction = fileMenu->addAction(tr("Save"), this, &MainWindow::saveFile, QKeySequence::Save);
}

void MainWindow::initializeComponents() {
    subject = std::make_shared<DocumentSubject>();
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
    return std::make_shared<BoldDecorator>(italicText);
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
    QString filter = tr("All Supported Files (*.txt *.html *.xml *.rtf);;Text Files (*.txt);;HTML Files (*.html);;XML Files (*.xml);;RTF Files (*.rtf);;All Files (*.*)");
    QString filePath = QFileDialog::getOpenFileName(
        this, tr("Open File"),
        QString(),
        filter
    );
    
    if (filePath.isEmpty()) return;

    openFileAtPath(filePath);
}

void MainWindow::openFileAtPath(const QString& filePath) {
    QString format = QFileInfo(filePath).suffix().toLower();
    
    // Создаем соответствующий адаптер в зависимости от формата файла
    std::shared_ptr<IDocumentAdapter> adapter;
    if (format == "html") {
        adapter = std::make_shared<HtmlDocumentAdapter>();
    } else if (format == "xml") {
        adapter = std::make_shared<XmlDocumentAdapter>();
    } else if (format == "rtf") {
        adapter = std::make_shared<RtfDocumentAdapter>();
    } else {
        adapter = std::make_shared<TextDocumentAdapter>();
    }

    try {
        QString content = adapter->loadDocument(filePath);
        
        QTextEdit* textEdit = new QTextEdit();
        connect(textEdit, &QTextEdit::textChanged, this, &MainWindow::onTextChanged);
        textEdit->setPlainText(content);
        auto editor = createTextComponent(textEdit);

        editors.push_back(editor);
        filePaths.push_back(filePath);
        receivers.push_back(std::make_shared<TextReceiver>());
        commandHistory.push_back({});
        commandIndex.push_back(-1);

        int index = tabs->addTab(textEdit, QFileInfo(filePath).fileName());
        tabs->setCurrentIndex(index);
        
        // Устанавливаем начальное состояние документа как сохраненный
        editorContext->setState(std::make_shared<SavedState>());
        editorContext->requestEdit("Load");
    } catch (const std::exception& e) {
        QMessageBox::warning(this, tr("Error"), tr("Failed to open file: %1").arg(e.what()));
    }
}

void MainWindow::saveFile() {
    if (currentIndex < 0) return;

    QString& path = filePaths[currentIndex];
    if (path.isEmpty()) {
        QString filter = tr("Text Files (*.txt);;HTML Files (*.html);;XML Files (*.xml);;RTF Files (*.rtf);;All Files (*.*)");
        QString selectedFilter;
        path = QFileDialog::getSaveFileName(
            this, tr("Save File"),
            QString(),
            filter,
            &selectedFilter
        );
        if (path.isEmpty()) return;
        
        // Добавляем расширение, если оно не указано
        if (!path.contains(".")) {
            if (selectedFilter.contains("*.txt")) path += ".txt";
            else if (selectedFilter.contains("*.html")) path += ".html";
            else if (selectedFilter.contains("*.xml")) path += ".xml";
            else if (selectedFilter.contains("*.rtf")) path += ".rtf";
        }
    }

    saveFileToPath(path);
}

void MainWindow::saveFileToPath(const QString& path) {
    QTextEdit* editorWidget = qobject_cast<QTextEdit*>(tabs->currentWidget());
    if (!editorWidget) return;

    QString content = editorWidget->toPlainText();
    QString format = QFileInfo(path).suffix().toLower();
    
    std::shared_ptr<IDocumentAdapter> adapter;
    if (format == "html") {
        adapter = std::make_shared<HtmlDocumentAdapter>();
    } else if (format == "xml") {
        adapter = std::make_shared<XmlDocumentAdapter>();
    } else if (format == "rtf") {
        adapter = std::make_shared<RtfDocumentAdapter>();
    } else {
        adapter = std::make_shared<TextDocumentAdapter>();
    }

    try {
        adapter->saveDocument(path, content);
        tabs->setTabText(currentIndex, QFileInfo(path).fileName());
        filePaths[currentIndex] = path;
        updateWindowTitle();
        
        // Устанавливаем состояние "Saved" после успешного сохранения
        editorContext->setState(std::make_shared<SavedState>());
        updateDocumentState();
    } catch (const std::exception& e) {
        QMessageBox::warning(this, tr("Error"), tr("Failed to save file: %1").arg(e.what()));
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
    
    // При изменении текста устанавливаем состояние "Modified"
    if (editorContext) {
        editorContext->setState(std::make_shared<ModifiedState>());
        updateDocumentState();
    }
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
