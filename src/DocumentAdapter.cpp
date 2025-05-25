#include "DocumentAdapter.hpp"

QString TextDocument::readTextFile(const QString& filePath) {
    // Simplified: In reality, read from file
    return "Text content from " + filePath;
}

void TextDocument::writeTextFile(const QString& filePath, const QString& content) {
    // Simplified: In reality, write to file
}

QString RtfDocument::readRtfFile(const QString& filePath) {
    // Simplified
    return "RTF content from " + filePath;
}

void RtfDocument::writeRtfFile(const QString& filePath, const QString& content) {
    // Simplified
}

TextDocumentAdapter::TextDocumentAdapter() {}

QString TextDocumentAdapter::loadDocument(const QString& filePath) {
    return textDoc.readTextFile(filePath);
}

void TextDocumentAdapter::saveDocument(const QString& filePath, const QString& content) {
    textDoc.writeTextFile(filePath, content);
}

RtfDocumentAdapter::RtfDocumentAdapter() {}

QString RtfDocumentAdapter::loadDocument(const QString& filePath) {
    return rtfDoc.readRtfFile(filePath);
}

void RtfDocumentAdapter::saveDocument(const QString& filePath, const QString& content) {
    rtfDoc.writeRtfFile(filePath, content);
}