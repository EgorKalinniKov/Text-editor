#pragma once

#include <QString>
#include <memory>

// Command interface
class ICommand {
public:
    virtual ~ICommand() = default;
    virtual void execute() = 0;
    virtual void undo() = 0;
};

// Receiver interface
class ITextReceiver {
public:
    virtual ~ITextReceiver() = default;
    virtual void setText(const QString& newText) = 0;
    virtual QString getText() const = 0;
};

// Concrete Receiver
class TextReceiver : public ITextReceiver {
public:
    explicit TextReceiver(const QString& text = QString());
    void setText(const QString& newText) override;
    QString getText() const override;
    
    // Запрет копирования
    TextReceiver(const TextReceiver&) = delete;
    TextReceiver& operator=(const TextReceiver&) = delete;
    
    // Разрешаем перемещение
    TextReceiver(TextReceiver&&) noexcept = default;
    TextReceiver& operator=(TextReceiver&&) noexcept = default;

private:
    QString textContent;
};

// Command Invoker
class CommandInvoker {
public:
    void setCommand(std::shared_ptr<ICommand> command);
    void executeCommand();
    void undoCommand();

private:
    std::shared_ptr<ICommand> currentCommand;
};

// Concrete Commands
class InsertTextCommand : public ICommand {
public:
    InsertTextCommand(std::shared_ptr<ITextReceiver> receiver, 
                     const QString& newText,
                     int position);
    void execute() override;
    void undo() override;

private:
    std::shared_ptr<ITextReceiver> receiver;
    QString insertedText;
    int insertPosition;
};

class DeleteTextCommand : public ICommand {
public:
    DeleteTextCommand(std::shared_ptr<ITextReceiver> receiver,
                     int position,
                     int length);
    void execute() override;
    void undo() override;

private:
    std::shared_ptr<ITextReceiver> receiver;
    QString deletedText;
    int deletePosition;
    int deleteLength;
};

class ReplaceTextCommand : public ICommand {
public:
    ReplaceTextCommand(std::shared_ptr<ITextReceiver> receiver,
                      const QString& newText,
                      const QString& oldText);
    void execute() override;
    void undo() override;

private:
    std::shared_ptr<ITextReceiver> receiver;
    QString newText;
    QString oldText;
};