#include "TextDecorator.hpp"

SimpleTextEdit::SimpleTextEdit(QTextEdit* editor) : textEdit(editor) {}

QString SimpleTextEdit::getText() {
    return textEdit->toPlainText();
}

void SimpleTextEdit::setText(const QString& text) {
    textEdit->setPlainText(text);
}

TextDecorator::TextDecorator(std::shared_ptr<TextComponent> component) : wrapped(component) {}

QString TextDecorator::getText() {
    return wrapped->getText();
}

void TextDecorator::setText(const QString& text) {
    wrapped->setText(text);
}

SpellCheckDecorator::SpellCheckDecorator(std::shared_ptr<TextComponent> component)
    : TextDecorator(component) {}

QString SpellCheckDecorator::getText() {
    QString text = wrapped->getText();
    return checkSpelling(text);
}

void SpellCheckDecorator::setText(const QString& text) {
    wrapped->setText(text);
}

QString SpellCheckDecorator::checkSpelling(const QString& text) {
    return text;
}

#include "TextDecorator.hpp"

GrammarCheckDecorator::GrammarCheckDecorator(std::shared_ptr<TextComponent> component)
    : TextDecorator(component) {}

QString GrammarCheckDecorator::getText() {
    QString text = wrapped->getText();
    return checkGrammar(text);
}

void GrammarCheckDecorator::setText(const QString& text) {
    wrapped->setText(text);
}

QString GrammarCheckDecorator::checkGrammar(const QString& text) {
    return text;
}
