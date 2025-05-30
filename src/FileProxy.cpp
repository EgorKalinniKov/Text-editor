#include "FileProxy.hpp"
#include <QFile>
#include <QTextStream>
#include <QFileInfo>
#include <memory>
#include <unordered_map>

QString RealFileSubject::readFile(const QString& filePath) {
    QFile file(filePath);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        throw std::runtime_error("Cannot open file for reading");
    }

    QTextStream in(&file);
    in.setCodec("UTF-8");
    return in.readAll();
}

void RealFileSubject::writeFile(const QString& filePath, const QString& content) {
    QFile file(filePath);
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text)) {
        throw std::runtime_error("Cannot open file for writing");
    }

    QTextStream out(&file);
    out.setCodec("UTF-8");
    out << content;
}

// Virtual Proxy Implementation
VirtualFileProxy::VirtualFileProxy(const QString& filePath)
    : filePath(filePath), realSubject(nullptr), isLoaded(false), fileSize(0) {
    // Get file info without loading content
    QFileInfo fileInfo(filePath);
    if (fileInfo.exists()) {
        fileSize = fileInfo.size();
    }
}

QString VirtualFileProxy::readFile(const QString& path) {
    if (!isLoaded) {
        loadRealSubject();
    }
    return realSubject->readFile(path);
}

void VirtualFileProxy::writeFile(const QString& path, const QString& content) {
    if (!isLoaded) {
        loadRealSubject();
    }
    realSubject->writeFile(path, content);
}

void VirtualFileProxy::loadRealSubject() {
    if (!isLoaded) {
        realSubject = std::make_shared<RealFileSubject>();
        isLoaded = true;
    }
}

qint64 VirtualFileProxy::getFileSize() const {
    return fileSize;
}

bool VirtualFileProxy::isFileLoaded() const {
    return isLoaded;
}

// Smart Proxy Implementation
SmartFileProxy::SmartFileProxy() : realSubject(std::make_shared<RealFileSubject>()) {}

QString SmartFileProxy::readFile(const QString& filePath) {
    if (isFileLocked(filePath)) {
        throw std::runtime_error("File is locked by another process");
    }

    // Check cache first
    if (fileCache.contains(filePath)) {
        updateAccessTime(filePath);
        return fileCache[filePath];
    }

    // Read file and cache content
    QString content = realSubject->readFile(filePath);
    cacheFile(filePath, content);
    return content;
}

void SmartFileProxy::writeFile(const QString& filePath, const QString& content) {
    if (isFileLocked(filePath)) {
        throw std::runtime_error("File is locked by another process");
    }

    realSubject->writeFile(filePath, content);
    
    // Update cache
    cacheFile(filePath, content);
    
    // Add to modified files list
    modifiedFiles.insert(filePath);
}

void SmartFileProxy::lockFile(const QString& filePath) {
    lockedFiles.insert(filePath);
}

void SmartFileProxy::unlockFile(const QString& filePath) {
    lockedFiles.remove(filePath);
}

bool SmartFileProxy::isFileLocked(const QString& filePath) const {
    return lockedFiles.contains(filePath);
}

void SmartFileProxy::cacheFile(const QString& filePath, const QString& content) {
    fileCache[filePath] = content;
    fileAccessTimes[filePath] = QDateTime::currentDateTime();
    
    // Clean cache if it's too large
    if (fileCache.size() > maxCacheSize) {
        cleanCache();
    }
}

void SmartFileProxy::updateAccessTime(const QString& filePath) {
    fileAccessTimes[filePath] = QDateTime::currentDateTime();
}

void SmartFileProxy::cleanCache() {
    // Remove least recently used files from cache
    while (fileCache.size() > maxCacheSize) {
        QString oldestFile;
        QDateTime oldestTime = QDateTime::currentDateTime();
        
        for (auto it = fileAccessTimes.begin(); it != fileAccessTimes.end(); ++it) {
            if (it.value() < oldestTime) {
                oldestTime = it.value();
                oldestFile = it.key();
            }
        }
        
        if (!oldestFile.isEmpty()) {
            fileCache.remove(oldestFile);
            fileAccessTimes.remove(oldestFile);
        }
    }
}

QSet<QString> SmartFileProxy::getModifiedFiles() const {
    return modifiedFiles;
}

void SmartFileProxy::clearModifiedFlag(const QString& filePath) {
    modifiedFiles.remove(filePath);
}

void SmartFileProxy::clearCache() {
    fileCache.clear();
    fileAccessTimes.clear();
}