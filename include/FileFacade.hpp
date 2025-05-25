#ifndef FILEFACADE_HPP
#define FILEFACADE_HPP

#include <QString>
#include <memory>

class FileFacade {
public:
    FileFacade();
    QString loadFile(const QString& filePath, const QString& format);
    bool saveFile(const QString& path, const QString& content);

private:
    class FileFacadeImpl;
    std::shared_ptr<FileFacadeImpl> impl;
};

#endif // FILEFACADE_HPP