#ifndef DOCUMENTADAPTER_HPP
#define DOCUMENTADAPTER_HPP

#include <QString>

// Target interface
class DocumentTarget {
public:
    virtual ~DocumentTarget() = default;
    virtual QString loadDocument(const QString& filePath) = 0;
    virtual void saveDocument(const QString& filePath, const QString& content) = 0;
};

// Adaptee for .txt files
class TextDocument {
public:
    QString readTextFile(const QString& filePath);
    void writeTextFile(const QString& filePath, const QString& content);
};

// Adaptee for .rtf files (simplified)
class RtfDocument {
public:
    QString readRtfFile(const QString& filePath);
    void writeRtfFile(const QString& filePath, const QString& content);
};

// Adapter for .txt files
class TextDocumentAdapter : public DocumentTarget {
public:
    TextDocumentAdapter();
    QString loadDocument(const QString& filePath) override;
    void saveDocument(const QString& filePath, const QString& content) override;

private:
    TextDocument textDoc;
};

// Adapter for .rtf files
class RtfDocumentAdapter : public DocumentTarget {
public:
    RtfDocumentAdapter();
    QString loadDocument(const QString& filePath) override;
    void saveDocument(const QString& filePath, const QString& content) override;

private:
    RtfDocument rtfDoc;
};

#endif // DOCUMENTADAPTER_HPP