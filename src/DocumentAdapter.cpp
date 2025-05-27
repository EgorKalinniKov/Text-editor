#include "DocumentAdapter.hpp"
#include "FileHandler.hpp"
#include <QFile>
#include <QTextStream>
#include <stdexcept>

QString TextDocumentAdapter::loadDocument(const QString& filePath) {
    try {
        return FileHandler::getInstance().readFile(filePath);
    } catch (const std::exception& e) {
        throw std::runtime_error(QString("Text document load error: %1").arg(e.what()).toStdString());
    }
}

void TextDocumentAdapter::saveDocument(const QString& filePath, const QString& content) {
    if (!FileHandler::getInstance().writeFile(filePath, content)) {
        throw std::runtime_error("Failed to save text document");
    }
}

QString RtfDocumentAdapter::loadDocument(const QString& filePath) {
    try {
        QString content = FileHandler::getInstance().readFile(filePath);
        return stripRtfFormatting(content);
    } catch (const std::exception& e) {
        throw std::runtime_error(QString("RTF document load error: %1").arg(e.what()).toStdString());
    }
}

void RtfDocumentAdapter::saveDocument(const QString& filePath, const QString& content) {
    QString rtfContent = addRtfFormatting(content);
    if (!FileHandler::getInstance().writeFile(filePath, rtfContent)) {
        throw std::runtime_error("Failed to save RTF document");
    }
}

QString RtfDocumentAdapter::stripRtfFormatting(const QString& text) {
    // Здесь должна быть реализация удаления RTF форматирования
    return text.simplified();
}

QString RtfDocumentAdapter::addRtfFormatting(const QString& text) {
    // Здесь должна быть реализация добавления RTF форматирования
    return QString("{\\rtf1\\ansi\\deff0{\\fonttbl{\\f0\\fnil\\fcharset0 Arial;}}\n\\f0\\fs20 %1}")
        .arg(text);
}
