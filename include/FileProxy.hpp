#pragma once

#include <QString>
#include <QDateTime>
#include <QSet>
#include <QHash>
#include <memory>

// Subject Interface
class IFileSubject {
public:
    virtual ~IFileSubject() = default;
    virtual QString readFile(const QString& filePath) = 0;
    virtual void writeFile(const QString& filePath, const QString& content) = 0;
};

// Real Subject
class RealFileSubject : public IFileSubject {
public:
    QString readFile(const QString& filePath) override;
    void writeFile(const QString& filePath, const QString& content) override;
};

// Virtual Proxy
class VirtualFileProxy : public IFileSubject {
public:
    explicit VirtualFileProxy(const QString& filePath);
    
    QString readFile(const QString& path) override;
    void writeFile(const QString& path, const QString& content) override;
    
    // Additional functionality
    qint64 getFileSize() const;
    bool isFileLoaded() const;

private:
    void loadRealSubject();
    
    QString filePath;
    std::shared_ptr<RealFileSubject> realSubject;
    bool isLoaded;
    qint64 fileSize;
};

// Smart Proxy
class SmartFileProxy : public IFileSubject {
public:
    SmartFileProxy();
    
    QString readFile(const QString& filePath) override;
    void writeFile(const QString& filePath, const QString& content) override;
    
    // File locking
    void lockFile(const QString& filePath);
    void unlockFile(const QString& filePath);
    bool isFileLocked(const QString& filePath) const;
    
    // Cache management
    void clearCache();
    QSet<QString> getModifiedFiles() const;
    void clearModifiedFlag(const QString& filePath);

private:
    void cacheFile(const QString& filePath, const QString& content);
    void updateAccessTime(const QString& filePath);
    void cleanCache();
    
    std::shared_ptr<RealFileSubject> realSubject;
    QHash<QString, QString> fileCache;
    QHash<QString, QDateTime> fileAccessTimes;
    QSet<QString> lockedFiles;
    QSet<QString> modifiedFiles;
    
    static const int maxCacheSize = 100; // Maximum number of files in cache
};