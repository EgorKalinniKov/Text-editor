#include "TextDecorator.hpp"
#include <QRegularExpression>

SimpleTextEdit::SimpleTextEdit(QTextEdit* editor) : textEdit(editor) {
    if (!editor) {
        throw std::invalid_argument("Editor cannot be null");
    }
}

QString SimpleTextEdit::getText() {
    return textEdit->toPlainText();
}

void SimpleTextEdit::setText(const QString& text) {
    textEdit->setPlainText(text);
}

TextDecorator::TextDecorator(std::shared_ptr<TextComponent> component) 
    : wrapped(component) {
    if (!component) {
        throw std::invalid_argument("Component cannot be null");
    }
}

QString TextDecorator::getText() {
    return wrapped->getText();
}

void TextDecorator::setText(const QString& text) {
    wrapped->setText(text);
}

SpellCheckDecorator::SpellCheckDecorator(std::shared_ptr<TextComponent> component)
    : TextDecorator(component), dictionary(loadDictionary()) {}

QString SpellCheckDecorator::getText() {
    QString text = wrapped->getText();
    return spellCheck(text);
}

void SpellCheckDecorator::setText(const QString& text) {
    QString checkedText = spellCheck(text);
    wrapped->setText(checkedText);
}

QString SpellCheckDecorator::spellCheck(const QString& text) {
    // В реальном приложении здесь была бы проверка орфографии
    // Сейчас просто демонстрационная реализация
    QString result = text;
    for (const auto& word : text.split(QRegularExpression("\\s+"))) {
        if (!dictionary.contains(word.toLower())) {
            result.replace(word, QString("*%1*").arg(word));
        }
    }
    return result;
}

QSet<QString> SpellCheckDecorator::loadDictionary() {
    // В реальном приложении здесь была бы загрузка словаря
    return QSet<QString>{"the", "a", "an", "in", "on", "at", "to", "for"};
}

GrammarCheckDecorator::GrammarCheckDecorator(std::shared_ptr<TextComponent> component)
    : TextDecorator(component) {}

QString GrammarCheckDecorator::getText() {
    QString text = wrapped->getText();
    return checkGrammar(text);
}

void GrammarCheckDecorator::setText(const QString& text) {
    QString checkedText = checkGrammar(text);
    wrapped->setText(checkedText);
}

QString GrammarCheckDecorator::checkGrammar(const QString& text) {
    // В реальном приложении здесь была бы проверка грамматики
    // Сейчас просто демонстрационная реализация
    QString result = text;
    
    // Проверка двойных пробелов
    result.replace(QRegularExpression("\\s{2,}"), " ");
    
    // Проверка пунктуации
    result.replace(QRegularExpression("\\s+([.,!?])"), "\\1");
    
    // Заглавная буква в начале предложения
    QStringList sentences = result.split(QRegularExpression("[.!?]\\s*"));
    for (QString& sentence : sentences) {
        if (!sentence.isEmpty()) {
            sentence[0] = sentence[0].toUpper();
        }
    }
    
    return sentences.join(". ");
}
