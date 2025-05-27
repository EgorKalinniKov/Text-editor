#pragma once

#include <QString>
#include <memory>
#include "DocumentAdapter.hpp"

class FileFacade {
public:
    FileFacade();
    ~FileFacade();

    QString loadFile(const QString& filePath, const QString& format);
    bool saveFile(const QString& filePath, const QString& content);

private:
    class FileFacadeImpl {
    public:
        struct FileOperation {
            QString filePath;
            QString format;
            QString content;
        };

        QString loadFile(const QString& filePath, const QString& format);
        bool saveFile(const QString& filePath, const QString& content);

    private:
        QString getFileFormat(const QString& filePath);
        std::unique_ptr<DocumentTarget> createAdapter(const FileOperation& op);
    };

    std::unique_ptr<FileFacadeImpl> impl;
};