#include "FileFacade.hpp"
#include <QFile>
#include <QTextStream>
#include <QFileInfo>
#include <QCoreApplication>

QString FileFacade::FileFacadeImpl::loadFile(const QString& filePath, const QString& format) {
    FileOperation op{filePath, format, QString()};
    
    auto adapter = createAdapter(op);
    if (!adapter) {
        return QCoreApplication::translate("FileFacade", "Unsupported format");
    }

    try {
        return adapter->loadDocument(filePath);
    } catch (const std::exception& e) {
        return QString("Error: %1").arg(e.what());
    }
}

bool FileFacade::FileFacadeImpl::saveFile(const QString& filePath, const QString& content) {
    FileOperation op{filePath, getFileFormat(filePath), content};
    
    auto adapter = createAdapter(op);
    if (!adapter) {
        return false;
    }

    try {
        adapter->saveDocument(filePath, content);
        return true;
    } catch (const std::exception& e) {
        return false;
    }
}

QString FileFacade::FileFacadeImpl::getFileFormat(const QString& filePath) {
    return "." + QFileInfo(filePath).suffix().toLower();
}

std::unique_ptr<DocumentTarget> FileFacade::FileFacadeImpl::createAdapter(const FileOperation& op) {
    if (op.format == ".txt") {
        return std::make_unique<TextDocumentAdapter>();
    } else if (op.format == ".rtf") {
        return std::make_unique<RtfDocumentAdapter>();
    }
    return nullptr;
}

FileFacade::FileFacade() : impl(std::make_unique<FileFacadeImpl>()) {}

FileFacade::~FileFacade() = default;

QString FileFacade::loadFile(const QString& filePath, const QString& format) {
    return impl->loadFile(filePath, format);
}

bool FileFacade::saveFile(const QString& filePath, const QString& content) {
    return impl->saveFile(filePath, content);
}
