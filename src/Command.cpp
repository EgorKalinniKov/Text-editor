#include "Command.hpp"
#include <stdexcept>

TextReceiver::TextReceiver(const QString& text) : textContent(text) {}

void TextReceiver::setText(const QString& newText) {
    textContent = newText;
}

QString TextReceiver::getText() const {
    return textContent;
}

void CommandInvoker::setCommand(std::shared_ptr<ICommand> command) {
    if (!command) {
        throw std::invalid_argument("Command cannot be null");
    }
    currentCommand = command;
}

void CommandInvoker::executeCommand() {
    if (currentCommand) {
        currentCommand->execute();
    }
}

void CommandInvoker::undoCommand() {
    if (currentCommand) {
        currentCommand->undo();
    }
}

InsertTextCommand::InsertTextCommand(std::shared_ptr<ITextReceiver> receiver,
                                   const QString& newText,
                                   int position)
    : receiver(receiver), insertedText(newText), insertPosition(position) {
    if (!receiver) {
        throw std::invalid_argument("Receiver cannot be null");
    }
}

void InsertTextCommand::execute() {
    QString text = receiver->getText();
    text.insert(insertPosition, insertedText);
    receiver->setText(text);
}

void InsertTextCommand::undo() {
    QString text = receiver->getText();
    text.remove(insertPosition, insertedText.length());
    receiver->setText(text);
}

DeleteTextCommand::DeleteTextCommand(std::shared_ptr<ITextReceiver> receiver,
                                   int position,
                                   int length)
    : receiver(receiver), deletePosition(position), deleteLength(length) {
    if (!receiver) {
        throw std::invalid_argument("Receiver cannot be null");
    }
    QString text = receiver->getText();
    deletedText = text.mid(position, length);
}

void DeleteTextCommand::execute() {
    QString text = receiver->getText();
    text.remove(deletePosition, deleteLength);
    receiver->setText(text);
}

void DeleteTextCommand::undo() {
    QString text = receiver->getText();
    text.insert(deletePosition, deletedText);
    receiver->setText(text);
}

ReplaceTextCommand::ReplaceTextCommand(std::shared_ptr<ITextReceiver> receiver,
                                     const QString& newText,
                                     const QString& oldText)
    : receiver(receiver), newText(newText), oldText(oldText) {
    if (!receiver) {
        throw std::invalid_argument("Receiver cannot be null");
    }
}

void ReplaceTextCommand::execute() {
    QString text = receiver->getText();
    text.replace(oldText, newText);
    receiver->setText(text);
}

void ReplaceTextCommand::undo() {
    QString text = receiver->getText();
    text.replace(newText, oldText);
    receiver->setText(text);
}