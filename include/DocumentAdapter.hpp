#pragma once

#include <QString>
#include <memory>
#include <stdexcept>
#include "FileHandler.hpp"

// Целевой интерфейс (Target)
class DocumentTarget {
public:
    virtual ~DocumentTarget() = default;
    virtual QString loadDocument(const QString& filePath) = 0;
    virtual void saveDocument(const QString& filePath, const QString& content) = 0;
};

// Адаптер для txt файлов (Concrete Adapter)
class TextDocumentAdapter : public DocumentTarget {
public:
    QString loadDocument(const QString& filePath) override;
    void saveDocument(const QString& filePath, const QString& content) override;
};

// Адаптер для rtf файлов (Concrete Adapter)
class RtfDocumentAdapter : public DocumentTarget {
public:
    QString loadDocument(const QString& filePath) override;
    void saveDocument(const QString& filePath, const QString& content) override;

private:
    QString stripRtfFormatting(const QString& content);
    QString addRtfFormatting(const QString& content);
};
