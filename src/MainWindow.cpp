#include "MainWindow.hpp"
#include <QMenuBar>
#include <QFileDialog>
#include <QMessageBox>
#include <QCloseEvent>
#include <QFileInfo>

MainWindow* MainWindow::instance = nullptr;

MainWindow* MainWindow::getInstance() {
    if (!instance) {
        instance = new MainWindow();
    }
    return instance;
}

MainWindow::MainWindow() {
    resize(1200, 900);
    tabs = new QTabWidget(this);
    tabs->setTabsClosable(true);
    setCentralWidget(tabs);

    connect(tabs, &QTabWidget::tabCloseRequested, this, &MainWindow::closeTab);

    QMenu* fileMenu = menuBar()->addMenu("File");
    fileMenu->addAction("Open", this, &MainWindow::openFile);
    saveAction = fileMenu->addAction("Save", this, &MainWindow::saveFile);
    fileMenu->addAction("New", this, &MainWindow::newFile);

    saveAction->setShortcut(QKeySequence(Qt::CTRL + Qt::Key_S));

    QMenu* editMenu = menuBar()->addMenu("Edit");
    editMenu->addAction("Undo", this, &MainWindow::undo);
    editMenu->addAction("Redo", this, &MainWindow::redo);
    editMenu->addAction("Count Characters", this, &MainWindow::countCharacters);
    editMenu->addAction("Toggle Mode", this, &MainWindow::toggleMode);

    subject = std::make_shared<DocumentSubject>();
    fileFacade = std::make_shared<FileFacade>();
    editorContext = std::make_shared<EditorContext>();
}

void MainWindow::newFile() {
    QTextEdit* textEdit = new QTextEdit();
    auto simpleText = std::make_shared<SimpleTextEdit>(textEdit);
    auto spellChecked = std::make_shared<SpellCheckDecorator>(simpleText);
    auto grammarChecked = std::make_shared<GrammarCheckDecorator>(spellChecked);

    editors.push_back(grammarChecked);
    receivers.push_back(std::make_shared<TextReceiver>(""));
    filePaths.push_back("");
    commandHistory.push_back({});
    commandIndex.push_back(-1);

    tabs->addTab(textEdit, "Untitled");
    tabs->setCurrentWidget(textEdit);
}

void MainWindow::closeTab(int index) {
    tabs->removeTab(index);
    editors.removeAt(index);
    filePaths.removeAt(index);
    receivers.removeAt(index);
    commandHistory.removeAt(index);
    commandIndex.removeAt(index);
}

void MainWindow::closeEvent(QCloseEvent* event) {
    tabs->clear();
    editors.clear();
    filePaths.clear();
    receivers.clear();
    commandHistory.clear();
    commandIndex.clear();
    QMainWindow::closeEvent(event);
}

void MainWindow::openFile() {
    QString filePath = QFileDialog::getOpenFileName(this, "Open File");
    if (filePath.isEmpty()) return;

    QString format = QFileInfo(filePath).suffix();
    QString content = fileFacade->loadFile(filePath, "." + format);

    QTextEdit* textEdit = new QTextEdit();
    auto simpleText = std::make_shared<SimpleTextEdit>(textEdit);
    auto decorated = std::make_shared<SpellCheckDecorator>(simpleText);  // Or chain more
    textEdit->setText(content);

    editors.push_back(decorated);
    filePaths.push_back(filePath);
    receivers.push_back(std::make_shared<TextReceiver>(content));
    commandHistory.push_back({});
    commandIndex.push_back(-1);

    tabs->addTab(textEdit, QFileInfo(filePath).fileName());
}

void MainWindow::saveFile() {
    int index = tabs->currentIndex();
    if (index < 0 || index >= tabs->count()) return;

    QString& path = filePaths[index];

    // Если путь не задан — спрашиваем пользователя
    if (path.isEmpty()) {
        path = QFileDialog::getSaveFileName(this, "Save File");
        if (path.isEmpty()) return; // Отмена
    }

    // Получаем текст из QTextEdit
    QTextEdit* editorWidget = qobject_cast<QTextEdit*>(tabs->widget(index));
    if (!editorWidget) return;

    QString content = editorWidget->toPlainText();

    // Сохраняем через фасад
    if (fileFacade && fileFacade->saveFile(path, content)) {
        tabs->setTabText(index, QFileInfo(path).fileName());
    } else {
        QMessageBox::warning(this, "Ошибка", "Не удалось сохранить файл.");
    }
}

void MainWindow::undo() {
    int index = tabs->currentIndex();
    if (index < 0 || commandIndex[index] < 0) return;

    commandHistory[index][commandIndex[index]]->undo();
    commandIndex[index]--;
}

void MainWindow::redo() {
    int index = tabs->currentIndex();
    if (index < 0 || commandIndex[index] >= commandHistory[index].size() - 1) return;

    commandIndex[index]++;
    commandHistory[index][commandIndex[index]]->execute();
}

void MainWindow::countCharacters() {
    int index = tabs->currentIndex();
    if (index < 0) return;

    QString text = editors[index]->getText();
    auto aggregate = std::make_shared<ConcreteTextAggregate>(text);
    auto it = aggregate->createIterator();

    int count = 0;
    while (it->hasNext()) {
        it->next();
        count++;
    }

    QMessageBox::information(this, "Characters", "Total characters: " + QString::number(count));
}

void MainWindow::toggleMode() {
    if (editorContext->getStatus().contains("read-only")) {
        editorContext->setState(std::make_shared<EditableState>());
    } else {
        editorContext->setState(std::make_shared<ReadOnlyState>());
    }

    editorContext->requestEdit("Toggle");
    QMessageBox::information(this, "Mode", editorContext->getStatus());
}
