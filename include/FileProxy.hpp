#ifndef FILEPROXY_HPP
#define FILEPROXY_HPP

#include <QString>
#include <memory>

// Real Subject
class RealFileSubject {
public:
    QString readFile(const QString& filePath);
    void writeFile(const QString& filePath, const QString& content);
};

// Proxy
class FileAccessProxy : public RealFileSubject {
public:
    FileAccessProxy();
    QString readFile(const QString& filePath);
    void writeFile(const QString& filePath, const QString& content);

private:
    std::shared_ptr<RealFileSubject> realSubject;
    bool checkAccess() const;
};

#endif // FILEPROXY_HPP