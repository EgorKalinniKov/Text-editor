#include "FileFacade.hpp"
#include "DocumentAdapter.hpp"
#include <QFile>
#include <QTextStream>

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
            return; // неподдерживаемый формат
        }
        adapter->saveDocument(filePath, content);
    }
};

FileFacade::FileFacade() : impl(std::make_shared<FileFacadeImpl>()) {}

QString FileFacade::loadFile(const QString& filePath, const QString& format) {
    return impl->loadFile(filePath, format);
}

bool FileFacade::saveFile(const QString& path, const QString& content) {
    QFile file(path);
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text)) return false;
    QTextStream out(&file);
    out.setCodec("UTF-8");
    out << content;
    return true;
}
