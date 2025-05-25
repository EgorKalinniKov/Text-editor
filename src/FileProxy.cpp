#include "FileProxy.hpp"

QString RealFileSubject::readFile(const QString& filePath) {
    // Simplified
    return "Real content from " + filePath;
}

void RealFileSubject::writeFile(const QString& filePath, const QString& content) {
    // Simplified
}

FileAccessProxy::FileAccessProxy() : realSubject(std::make_shared<RealFileSubject>()) {}

bool FileAccessProxy::checkAccess() const {
    // Simplified access check
    return true;
}

QString FileAccessProxy::readFile(const QString& filePath) {
    if (checkAccess()) {
        return realSubject->readFile(filePath);
    }
    return "Access denied";
}

void FileAccessProxy::writeFile(const QString& filePath, const QString& content) {
    if (checkAccess()) {
        realSubject->writeFile(filePath, content);
    }
}