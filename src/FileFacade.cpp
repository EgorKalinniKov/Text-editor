#include "FileFacade.hpp"
#include "DocumentAdapter.hpp"

class FileFacade::FileFacadeImpl {
public:
    QString loadFile(const QString& filePath, const QString& format) {
        std::unique_ptr<DocumentTarget> adapter;
        if (format == ".txt") {
            adapter = std::make_unique<TextDocumentAdapter>();
        } else if (format == ".rtf") {
            adapter = std::make_unique<RtfDocumentAdapter>();
        } else {
            return "Unsupported format";
        }
        return adapter->loadDocument(filePath);
    }

    void saveFile(const QString& filePath, const QString& content, const QString& format) {
        std::unique_ptr<DocumentTarget> adapter;
        if (format == ".txt") {
            adapter = std::make_unique<TextDocumentAdapter>();
        } else if (format == ".rtf") {
            adapter = std::make_unique<RtfDocumentAdapter>();
        } else {
            return;
        }
        adapter->saveDocument(filePath, content);
    }
};

FileFacade::FileFacade() : impl(std::make_shared<FileFacadeImpl>()) {}

QString FileFacade::loadFile(const QString& filePath, const QString& format) {
    return impl->loadFile(filePath, format);
}

void FileFacade::saveFile(const QString& filePath, const QString& content, const QString& format) {
    impl->saveFile(filePath, content, format);
}