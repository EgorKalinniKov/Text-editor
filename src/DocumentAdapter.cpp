#include "DocumentAdapter.hpp"
#include "FileHandler.hpp"
#include <QRegularExpression>

QString TextDocumentAdapter::loadDocument(const QString& filePath) {
    try {
        return FileHandler::readFile(filePath);
    } catch (const std::exception& e) {
        return QString("Error: %1").arg(e.what());
    }
}

void TextDocumentAdapter::saveDocument(const QString& filePath, const QString& content) {
    try {
        FileHandler::writeFile(filePath, content);
    } catch (const std::exception& e) {
        throw; // Пробрасываем исключение выше для обработки в Facade
    }
}

QString RtfDocumentAdapter::loadDocument(const QString& filePath) {
    try {
        QString content = FileHandler::readFile(filePath);
        return stripRtfFormatting(content);
    } catch (const std::exception& e) {
        return QString("Error: %1").arg(e.what());
    }
}

void RtfDocumentAdapter::saveDocument(const QString& filePath, const QString& content) {
    try {
        QString rtfContent = addRtfFormatting(content);
        FileHandler::writeFile(filePath, rtfContent);
    } catch (const std::exception& e) {
        throw; // Пробрасываем исключение выше для обработки в Facade
    }
}

QString RtfDocumentAdapter::stripRtfFormatting(const QString& content) {
    // Здесь должна быть реальная логика удаления RTF форматирования
    return content;
}

QString RtfDocumentAdapter::addRtfFormatting(const QString& content) {
    // Здесь должна быть реальная логика добавления RTF форматирования
    return QString("{\\rtf1\\ansi\\deff0{\\fonttbl{\\f0\\fnil\\fcharset0 Arial;}}\n"
                  "\\viewkind4\\uc1\\pard\\lang1033\\f0\\fs20 %1}").arg(content);
}
