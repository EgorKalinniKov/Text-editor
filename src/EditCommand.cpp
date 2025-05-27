#include "EditCommand.hpp"

TextReceiver::TextReceiver(const QString& text) : textContent(text) {}


void TextReceiver::setText(const QString& newText) {
    textContent = newText;
}

QString TextReceiver::getText() const {
    return textContent;
}

// CommandInvoker implementation
void CommandInvoker::setCommand(std::shared_ptr<EditCommand> command) {
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

// InsertTextCommand implementation
InsertTextCommand::InsertTextCommand(std::shared_ptr<TextReceiver> receiver,
                                   const QString& newText,
                                   int position)
    : receiver(receiver)
    , insertedText(newText)
    , insertPosition(position) {
    if (!receiver) {
        throw std::invalid_argument("Receiver cannot be null");
    }
    if (position < 0 || position > receiver->getText().length()) {
        throw std::out_of_range("Invalid insert position");
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

// DeleteTextCommand implementation
DeleteTextCommand::DeleteTextCommand(std::shared_ptr<TextReceiver> receiver,
                                   int position,
                                   int length)
    : receiver(receiver)
    , deletePosition(position)
    , deleteLength(length) {
    if (!receiver) {
        throw std::invalid_argument("Receiver cannot be null");
    }
    QString text = receiver->getText();
    if (position < 0 || position + length > text.length()) {
        throw std::out_of_range("Invalid delete range");
    }
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

// ReplaceTextCommand implementation
ReplaceTextCommand::ReplaceTextCommand(std::shared_ptr<TextReceiver> receiver,
                                     const QString& newText,
                                     const QString& oldText)
    : receiver(receiver)
    , newText(newText)
    , oldText(oldText) {
    if (!receiver) {
        throw std::invalid_argument("Receiver cannot be null");
    }
}

void ReplaceTextCommand::execute() {
    receiver->setText(newText);
}

void ReplaceTextCommand::undo() {
    receiver->setText(oldText);
}