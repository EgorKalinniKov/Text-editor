#include "FileProxy.hpp"
#include <QFile>
#include <QTextStream>

QString RealFileSubject::readFile(const QString& filePath) {
    QFile file(filePath);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) return "";

    QTextStream in(&file);
    in.setCodec("UTF-8");
    return in.readAll();
}

void RealFileSubject::writeFile(const QString& filePath, const QString& content) {
    QFile file(filePath);
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text)) return;

    QTextStream out(&file);
    out.setCodec("UTF-8");
    out << content;
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