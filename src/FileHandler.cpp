#include "FileHandler.hpp"
#include <QFile>
#include <QTextStream>
#include <stdexcept>

QString FileHandler::readFile(const QString& filePath) {
    QFile file(filePath);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        throw std::runtime_error("Failed to open file for reading");
    }

    QTextStream in(&file);
    in.setCodec("UTF-8");
    QString content = in.readAll();
    file.close();
    return content;
}

void FileHandler::writeFile(const QString& filePath, const QString& content) {
    QFile file(filePath);
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text)) {
        throw std::runtime_error("Failed to open file for writing");
    }

    QTextStream out(&file);
    out.setCodec("UTF-8");
    out << content;
    file.close();
} 