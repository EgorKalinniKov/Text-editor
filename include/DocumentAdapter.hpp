#pragma once

#include <QString>

// Abstract Document Adapter
class IDocumentAdapter {
public:
    virtual ~IDocumentAdapter() = default;
    virtual QString loadDocument(const QString& filePath) = 0;
    virtual void saveDocument(const QString& filePath, const QString& content) = 0;
};

// Plain Text Adapter
class TextDocumentAdapter : public IDocumentAdapter {
public:
    QString loadDocument(const QString& filePath) override;
    void saveDocument(const QString& filePath, const QString& content) override;
};

// RTF Adapter
class RtfDocumentAdapter : public IDocumentAdapter {
public:
    QString loadDocument(const QString& filePath) override;
    void saveDocument(const QString& filePath, const QString& content) override;

private:
    QString stripRtfFormatting(const QString& rtfText);
    QString addRtfFormatting(const QString& text);
};

// HTML Adapter
class HtmlDocumentAdapter : public IDocumentAdapter {
public:
    QString loadDocument(const QString& filePath) override;
    void saveDocument(const QString& filePath, const QString& content) override;

private:
    QString stripHtmlTags(const QString& htmlText);
    QString addHtmlTags(const QString& text);
};

// XML Adapter
class XmlDocumentAdapter : public IDocumentAdapter {
public:
    QString loadDocument(const QString& filePath) override;
    void saveDocument(const QString& filePath, const QString& content) override;

private:
    QString parseXmlContent(const QString& xmlText);
    QString createXmlContent(const QString& text);
};
