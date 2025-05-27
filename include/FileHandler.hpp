#pragma once

#include <QString>
#include <memory>

// File handler interface
class IFileHandler {
public:
    virtual ~IFileHandler() = default;
    virtual QString readFile(const QString& filePath) = 0;
    virtual bool writeFile(const QString& filePath, const QString& content) = 0;
};

// Concrete file handler
class FileHandler : public IFileHandler {
public:
    QString readFile(const QString& filePath) override;
    bool writeFile(const QString& filePath, const QString& content) override;

    // Singleton implementation
    static FileHandler& getInstance();

private:
    FileHandler() = default;
    FileHandler(const FileHandler&) = delete;
    FileHandler& operator=(const FileHandler&) = delete;
}; 