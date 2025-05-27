#pragma once

#include <QString>
#include <memory>
#include <stdexcept>

// Command interface
class EditCommand {
public:
    virtual ~EditCommand() = default;
    virtual void execute() = 0;
    virtual void undo() = 0;
};

// Receiver
class TextReceiver {
public:
    explicit TextReceiver(const QString& text = QString());
    virtual ~TextReceiver() = default;
    
    void setText(const QString& newText);
    QString getText() const;
    
    // Запрет копирования для безопасности
    TextReceiver(const TextReceiver&) = delete;
    TextReceiver& operator=(const TextReceiver&) = delete;
    
    // Разрешаем перемещение
    TextReceiver(TextReceiver&&) noexcept = default;
    TextReceiver& operator=(TextReceiver&&) noexcept = default;

private:
    QString textContent;
};

// Invoker
class CommandInvoker {
public:
    void setCommand(std::shared_ptr<EditCommand> command);
    void executeCommand();
    void undoCommand();

private:
    std::shared_ptr<EditCommand> currentCommand;
};

// Concrete Commands
class InsertTextCommand : public EditCommand {
public:
    InsertTextCommand(std::shared_ptr<TextReceiver> receiver, 
                     const QString& newText,
                     int position);
    void execute() override;
    void undo() override;

private:
    std::shared_ptr<TextReceiver> receiver;
    QString insertedText;
    int insertPosition;
};

class DeleteTextCommand : public EditCommand {
public:
    DeleteTextCommand(std::shared_ptr<TextReceiver> receiver,
                     int position,
                     int length);
    void execute() override;
    void undo() override;

private:
    std::shared_ptr<TextReceiver> receiver;
    QString deletedText;
    int deletePosition;
    int deleteLength;
};

class ReplaceTextCommand : public EditCommand {
public:
    ReplaceTextCommand(std::shared_ptr<TextReceiver> receiver,
                      const QString& newText,
                      const QString& oldText);
    void execute() override;
    void undo() override;

private:
    std::shared_ptr<TextReceiver> receiver;
    QString newText;
    QString oldText;
};