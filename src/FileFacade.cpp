#include "FileFacade.hpp"
#include <QFile>
#include <QTextStream>
#include <QFileInfo>
#include <QCoreApplication>

// Реализация методов FileFacadeImpl
QString FileFacadeImpl::loadFile(const QString& filePath, const QString& format) {
    QFile file(filePath);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        throw std::runtime_error(QCoreApplication::translate("FileFacade", "Cannot open file for reading").toStdString());
    }

    QTextStream in(&file);
    in.setCodec("UTF-8");  // Устанавливаем кодировку UTF-8
    QString content = in.readAll();
    file.close();
    return content;
}

bool FileFacadeImpl::saveFile(const QString& filePath, const QString& content) {
    QFile file(filePath);
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text)) {
        return false;
    }

    QTextStream out(&file);
    out.setCodec("UTF-8");  // Устанавливаем кодировку UTF-8
    out << content;
    out.flush();  // Принудительно записываем буфер
    file.close();
    return true;
}

// Реализация методов FileFacade
FileFacade::FileFacade() : pimpl(std::make_unique<FileFacadeImpl>()) {}

FileFacade::~FileFacade() = default;

QString FileFacade::loadFile(const QString& filePath, const QString& format) {
    return pimpl->loadFile(filePath, format);
}

bool FileFacade::saveFile(const QString& filePath, const QString& content) {
    return pimpl->saveFile(filePath, content);
}
