#pragma once

#include <QString>
#include <memory>

// Target interface
class IDocument {
public:
    virtual ~IDocument() = default;
    virtual QString loadDocument(const QString& filePath) = 0;
    virtual void saveDocument(const QString& filePath, const QString& content) = 0;
};

// Abstract adapter
class DocumentAdapter : public IDocument {
public:
    virtual ~DocumentAdapter() = default;
};

// Concrete adapters
class TextDocumentAdapter : public DocumentAdapter {
public:
    QString loadDocument(const QString& filePath) override;
    void saveDocument(const QString& filePath, const QString& content) override;
};

class RtfDocumentAdapter : public DocumentAdapter {
public:
    QString loadDocument(const QString& filePath) override;
    void saveDocument(const QString& filePath, const QString& content) override;

private:
    QString stripRtfFormatting(const QString& text);
    QString addRtfFormatting(const QString& text);
};
