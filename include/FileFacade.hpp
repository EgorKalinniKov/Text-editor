#pragma once

#include <QString>
#include <memory>

// Интерфейс для работы с файлами
class IFileFacade {
public:
    virtual ~IFileFacade() = default;
    virtual QString loadFile(const QString& filePath, const QString& format) = 0;
    virtual bool saveFile(const QString& filePath, const QString& content) = 0;
};

// Реализация интерфейса для работы с файлами
class FileFacadeImpl : public IFileFacade {
public:
    QString loadFile(const QString& filePath, const QString& format) override;
    bool saveFile(const QString& filePath, const QString& content) override;
};

// Фасад, предоставляющий упрощенный интерфейс для работы с файлами
class FileFacade : public IFileFacade {
public:
    FileFacade();
    ~FileFacade() override;

    QString loadFile(const QString& filePath, const QString& format) override;
    bool saveFile(const QString& filePath, const QString& content) override;

    // Запрет копирования
    FileFacade(const FileFacade&) = delete;
    FileFacade& operator=(const FileFacade&) = delete;

private:
    std::unique_ptr<FileFacadeImpl> pimpl;
};