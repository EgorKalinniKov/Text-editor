#include "DocumentAdapter.hpp"
#include <QFile>
#include <QTextStream>

QString TextDocumentAdapter::loadDocument(const QString& filePath) {
    QFile file(filePath);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
        return "Failed to open file";

    QTextStream in(&file);
    QString content = in.readAll();
    file.close();
    return content;
}

void TextDocumentAdapter::saveDocument(const QString& filePath, const QString& content) {
    QFile file(filePath);
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text))
        return;

    QTextStream out(&file);
    out << content;
    file.close();
}

QString RtfDocumentAdapter::loadDocument(const QString& filePath) {
    // Для простоты, чтение RTF как текста
    QFile file(filePath);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
        return "Failed to open file";

    QTextStream in(&file);
    QString content = in.readAll();
    file.close();
    return content;
}

void RtfDocumentAdapter::saveDocument(const QString& filePath, const QString& content) {
    QFile file(filePath);
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text))
        return;

    QTextStream out(&file);
    out << content;
    file.close();
}
