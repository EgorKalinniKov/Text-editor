#include "DocumentAdapter.hpp"
#include "FileHandler.hpp"
#include <QFile>
#include <QTextStream>
#include <QXmlStreamReader>
#include <QXmlStreamWriter>
#include <QRegularExpression>
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

// Improved RTF Adapter
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

QString RtfDocumentAdapter::stripRtfFormatting(const QString& rtfText) {
    QString text = rtfText;
    
    // Remove RTF header
    static QRegularExpression rtfHeader(R"(\{\\rtf1.*?\})");
    text.remove(rtfHeader);
    
    // Remove common RTF commands
    static QRegularExpression rtfCommands(R"(\\[a-zA-Z]+[-]?[0-9]*)");
    text.remove(rtfCommands);
    
    // Remove curly braces
    text.remove(QRegularExpression(R"(\{|\})"));
    
    // Remove special characters
    text.remove(QRegularExpression(R"(\\[\''][0-9a-fA-F]{2})"));
    
    return text.simplified();
}

QString RtfDocumentAdapter::addRtfFormatting(const QString& text) {
    QString rtfContent = "{\\rtf1\\ansi\\deff0{\\fonttbl{\\f0\\fnil\\fcharset0 Arial;}}\n";
    rtfContent += "\\viewkind4\\uc1\\pard\\lang1033\\f0\\fs24 ";
    
    // Escape special characters
    QString escaped = text;
    escaped.replace("\\", "\\\\");
    escaped.replace("{", "\\{");
    escaped.replace("}", "\\}");
    
    rtfContent += escaped;
    rtfContent += "}";
    return rtfContent;
}

// HTML Adapter
QString HtmlDocumentAdapter::loadDocument(const QString& filePath) {
    try {
        QString content = FileHandler::getInstance().readFile(filePath);
        return stripHtmlTags(content);
    } catch (const std::exception& e) {
        throw std::runtime_error(QString("HTML document load error: %1").arg(e.what()).toStdString());
    }
}

void HtmlDocumentAdapter::saveDocument(const QString& filePath, const QString& content) {
    QString htmlContent = addHtmlTags(content);
    if (!FileHandler::getInstance().writeFile(filePath, htmlContent)) {
        throw std::runtime_error("Failed to save HTML document");
    }
}

QString HtmlDocumentAdapter::stripHtmlTags(const QString& htmlText) {
    QString text = htmlText;
    
    // Remove DOCTYPE and HTML comments
    text.remove(QRegularExpression(R"(<!DOCTYPE[^>]*>|<!--[\s\S]*?-->)"));
    
    // Remove style tags and their content
    text.remove(QRegularExpression(R"(<style[^>]*>.*?</style>)", QRegularExpression::DotMatchesEverythingOption));
    
    // Remove HTML tags
    text.remove(QRegularExpression(R"(<[^>]*>)"));
    
    // Decode HTML entities
    text.replace("&amp;", "&");
    text.replace("&lt;", "<");
    text.replace("&gt;", ">");
    text.replace("&quot;", "\"");
    text.replace("&nbsp;", " ");
    
    return text.simplified();
}

QString HtmlDocumentAdapter::addHtmlTags(const QString& text) {
    QString htmlContent = "<!DOCTYPE html>\n<html>\n<head>\n";
    htmlContent += "<meta charset=\"UTF-8\">\n";
    htmlContent += "</head>\n<body>\n";
    
    // Convert newlines to paragraphs and preserve formatting
    QString content = text;
    QStringList paragraphs = content.split("\n");
    for (const QString& paragraph : paragraphs) {
        if (!paragraph.trimmed().isEmpty()) {
            htmlContent += "<p>" + paragraph.toHtmlEscaped() + "</p>\n";
        }
    }
    
    htmlContent += "</body>\n</html>";
    return htmlContent;
}

// XML Adapter
QString XmlDocumentAdapter::loadDocument(const QString& filePath) {
    try {
        QString content = FileHandler::getInstance().readFile(filePath);
        return parseXmlContent(content);
    } catch (const std::exception& e) {
        throw std::runtime_error(QString("XML document load error: %1").arg(e.what()).toStdString());
    }
}

void XmlDocumentAdapter::saveDocument(const QString& filePath, const QString& content) {
    QString xmlContent = createXmlContent(content);
    if (!FileHandler::getInstance().writeFile(filePath, xmlContent)) {
        throw std::runtime_error("Failed to save XML document");
    }
}

QString XmlDocumentAdapter::parseXmlContent(const QString& xmlText) {
    QXmlStreamReader reader(xmlText);
    QString content;
    
    while (!reader.atEnd() && !reader.hasError()) {
        if (reader.isCharacters() && !reader.isWhitespace()) {
            content += reader.text().toString() + "\n";
        }
        reader.readNext();
    }
    
    if (reader.hasError()) {
        throw std::runtime_error(QString("XML parsing error: %1").arg(reader.errorString()).toStdString());
    }
    
    return content.trimmed();
}

QString XmlDocumentAdapter::createXmlContent(const QString& text) {
    QString xmlContent;
    QXmlStreamWriter writer(&xmlContent);
    
    writer.setAutoFormatting(true);
    writer.writeStartDocument();
    writer.writeStartElement("document");
    writer.writeStartElement("content");
    
    // Split text into paragraphs
    QStringList paragraphs = text.split("\n");
    for (const QString& paragraph : paragraphs) {
        writer.writeStartElement("paragraph");
        writer.writeCharacters(paragraph);
        writer.writeEndElement(); // paragraph
    }
    
    writer.writeEndElement(); // content
    writer.writeEndElement(); // document
    writer.writeEndDocument();
    
    return xmlContent;
}
