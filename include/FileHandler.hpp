#pragma once

#include <QString>

class FileHandler {
public:
    virtual ~FileHandler() = default;
    static QString readFile(const QString& filePath);
    static void writeFile(const QString& filePath, const QString& content);

protected:
    FileHandler() = default;  // Защищенный конструктор, так как класс содержит только статические методы
}; 