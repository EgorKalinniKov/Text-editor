#include <gtest/gtest.h>
#include <QFile>
#include <QDir>
#include "DocumentAdapter.hpp"
#include <QTemporaryFile>

class DocumentAdapterTest : public ::testing::Test {
protected:
    void SetUp() override {
        textAdapter = std::make_shared<TextDocumentAdapter>();
        htmlAdapter = std::make_shared<HtmlDocumentAdapter>();
        xmlAdapter = std::make_shared<XmlDocumentAdapter>();
        rtfAdapter = std::make_shared<RtfDocumentAdapter>();
    }
    
    std::shared_ptr<TextDocumentAdapter> textAdapter;
    std::shared_ptr<HtmlDocumentAdapter> htmlAdapter;
    std::shared_ptr<XmlDocumentAdapter> xmlAdapter;
    std::shared_ptr<RtfDocumentAdapter> rtfAdapter;
    
    // Вспомогательная функция для сравнения строк
    void expectStringsEqual(const QString& actual, const QString& expected) {
        EXPECT_EQ(actual.toStdString(), expected.toStdString());
    }
    
    // Вспомогательная функция для очистки HTML
    QString cleanHtml(const QString& html) {
        QString result = html;
        // Сначала удаляем HTML теги
        result.remove(QRegExp("<[^>]*>"));
        // Заменяем множественные пробелы на один
        result.replace(QRegExp("\\s+"), " ");
        return result.trimmed();
    }
    
    // Вспомогательная функция для очистки RTF
    QString cleanRtf(const QString& rtf) {
        QString result = rtf;
        // Удаляем RTF команды и их параметры
        result.remove(QRegExp("\\\\[a-z]+[-]?[0-9]*"));
        // Удаляем фигурные скобки
        result.remove(QRegExp("[{}]"));
        // Удаляем одиночные обратные слеши
        result.remove(QRegExp("\\\\(?![\\\\])"));
        // Заменяем двойные обратные слеши на одиночные
        result.replace("\\\\", "\\");
        // Удаляем оставшиеся обратные слеши
        result.remove("\\");
        // Заменяем множественные пробелы на один
        result.replace(QRegExp("\\s+"), " ");
        return result.trimmed();
    }
};

TEST_F(DocumentAdapterTest, TextDocumentSaveAndLoad) {
    QString testContent = "Hello World Test";
    QString tempPath = QDir::tempPath() + "/test.txt";
    
    EXPECT_NO_THROW({
        textAdapter->saveDocument(tempPath, testContent);
        QString loadedContent = textAdapter->loadDocument(tempPath);
        expectStringsEqual(loadedContent.simplified(), testContent.simplified());
    });
    
    QFile::remove(tempPath);
}

TEST_F(DocumentAdapterTest, HtmlDocumentSaveAndLoad) {
    QString testContent = "<html><body>Hello World Test</body></html>";
    QString tempPath = QDir::tempPath() + "/test.html";
    
    EXPECT_NO_THROW({
        htmlAdapter->saveDocument(tempPath, testContent);
        QString loadedContent = htmlAdapter->loadDocument(tempPath);
        expectStringsEqual(cleanHtml(loadedContent), "Hello World Test");
    });
    
    QFile::remove(tempPath);
}

TEST_F(DocumentAdapterTest, XmlDocumentSaveAndLoad) {
    QString testContent = "<?xml version='1.0'?><root>Test</root>";
    QString tempPath = QDir::tempPath() + "/test.xml";
    
    EXPECT_NO_THROW({
        xmlAdapter->saveDocument(tempPath, testContent);
        QString loadedContent = xmlAdapter->loadDocument(tempPath);
        expectStringsEqual(loadedContent.simplified(), testContent.simplified());
    });
    
    QFile::remove(tempPath);
}

TEST_F(DocumentAdapterTest, RtfDocumentSaveAndLoad) {
    QString testContent = "{\\rtf1\\ansi Test}";
    QString tempPath = QDir::tempPath() + "/test.rtf";
    
    EXPECT_NO_THROW({
        rtfAdapter->saveDocument(tempPath, testContent);
        QString loadedContent = rtfAdapter->loadDocument(tempPath);
        expectStringsEqual(cleanRtf(loadedContent), "Test");
    });
    
    QFile::remove(tempPath);
}

TEST_F(DocumentAdapterTest, InvalidFilePath) {
    QString testContent = "Test Content";
    QString invalidPath = "/invalid/path/test.txt";
    
    EXPECT_THROW({
        textAdapter->saveDocument(invalidPath, testContent);
    }, std::runtime_error);
    
    EXPECT_THROW({
        textAdapter->loadDocument(invalidPath);
    }, std::runtime_error);
}

TEST_F(DocumentAdapterTest, EmptyContent) {
    QString tempPath = QDir::tempPath() + "/empty.txt";
    
    EXPECT_NO_THROW({
        textAdapter->saveDocument(tempPath, "");
        QString loadedContent = textAdapter->loadDocument(tempPath);
        EXPECT_TRUE(loadedContent.isEmpty());
    });
    
    QFile::remove(tempPath);
}

TEST_F(DocumentAdapterTest, FormatSpecificFeatures) {
    // HTML форматирование
    QString htmlContent = "<b>Bold</b> and <i>Italic</i>";
    QString tempHtmlPath = QDir::tempPath() + "/format.html";
    
    EXPECT_NO_THROW({
        htmlAdapter->saveDocument(tempHtmlPath, htmlContent);
        QString loadedHtmlContent = htmlAdapter->loadDocument(tempHtmlPath);
        expectStringsEqual(cleanHtml(loadedHtmlContent), "Bold and Italic");
    });
    QFile::remove(tempHtmlPath);
    
    // RTF форматирование
    QString rtfContent = "{\\b Bold} and {\\i Italic}";
    QString tempRtfPath = QDir::tempPath() + "/format.rtf";
    
    EXPECT_NO_THROW({
        rtfAdapter->saveDocument(tempRtfPath, rtfContent);
        QString loadedRtfContent = rtfAdapter->loadDocument(tempRtfPath);
        expectStringsEqual(cleanRtf(loadedRtfContent), "Bold and Italic");
    });
    QFile::remove(tempRtfPath);
} 