#include "EditCommand.hpp"

TextReceiver::TextReceiver(const QString& text) : textContent(text) {}


void TextReceiver::setText(const QString& newText) {
    textContent = newText;
}

QString TextReceiver::getText() const {
    return textContent;
}

EditTextCommand::EditTextCommand(std::shared_ptr<TextReceiver> receiver, const QString& newText, const QString& oldText)
    : receiver(receiver), newText(newText), oldText(oldText) {}

void EditTextCommand::execute() {
    receiver->setText(newText);
}

void EditTextCommand::undo() {
    receiver->setText(oldText);
}