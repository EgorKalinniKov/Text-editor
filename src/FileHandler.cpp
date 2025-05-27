#include "FileHandler.hpp"
#include <QFile>
#include <QTextStream>
#include <stdexcept>

FileHandler& FileHandler::getInstance() {
    static FileHandler instance;
    return instance;
}

QString FileHandler::readFile(const QString& filePath) {
    QFile file(filePath);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        throw std::runtime_error("Cannot open file for reading");
    }

    QTextStream in(&file);
    in.setCodec("UTF-8");
    QString content = in.readAll();
    file.close();
    return content;
}

bool FileHandler::writeFile(const QString& filePath, const QString& content) {
    QFile file(filePath);
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text)) {
        return false;
    }

    QTextStream out(&file);
    out.setCodec("UTF-8");
    out << content;
    out.flush();
    file.close();
    return true;
} 