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

QString SimpleTextEdit::getFormattedText() {
    return textEdit->toHtml();
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

QString TextDecorator::getFormattedText() {
    return wrapped->getFormattedText();
}

// ItalicDecorator implementation
ItalicDecorator::ItalicDecorator(std::shared_ptr<TextComponent> component)
    : TextDecorator(component) {}

QString ItalicDecorator::getText() {
    return removeItalicTags(wrapped->getText());
}

void ItalicDecorator::setText(const QString& text) {
    wrapped->setText(addItalicTags(text));
}

QString ItalicDecorator::getFormattedText() {
    return addItalicTags(wrapped->getFormattedText());
}

QString ItalicDecorator::addItalicTags(const QString& text) {
    return QString("<i>%1</i>").arg(text);
}

QString ItalicDecorator::removeItalicTags(const QString& text) {
    QString result = text;
    result.remove("<i>");
    result.remove("</i>");
    return result;
}

// ColorDecorator implementation
ColorDecorator::ColorDecorator(std::shared_ptr<TextComponent> component, const QColor& color)
    : TextDecorator(component), textColor(color) {}

QString ColorDecorator::getText() {
    return removeColorTags(wrapped->getText());
}

void ColorDecorator::setText(const QString& text) {
    wrapped->setText(addColorTags(text));
}

QString ColorDecorator::getFormattedText() {
    return addColorTags(wrapped->getFormattedText());
}

QString ColorDecorator::addColorTags(const QString& text) {
    return QString("<font color='%1'>%2</font>")
        .arg(textColor.name())
        .arg(text);
}

QString ColorDecorator::removeColorTags(const QString& text) {
    QString result = text;
    static QRegularExpression colorTagRegex("<font color='[^']*'>(.*?)</font>");
    QRegularExpressionMatch match = colorTagRegex.match(result);
    if (match.hasMatch()) {
        result = match.captured(1);
    }
    return result;
}

// BoldDecorator implementation
BoldDecorator::BoldDecorator(std::shared_ptr<TextComponent> component)
    : TextDecorator(component) {}

QString BoldDecorator::getText() {
    return removeBoldTags(wrapped->getText());
}

void BoldDecorator::setText(const QString& text) {
    wrapped->setText(addBoldTags(text));
}

QString BoldDecorator::getFormattedText() {
    return addBoldTags(wrapped->getFormattedText());
}

QString BoldDecorator::addBoldTags(const QString& text) {
    return QString("<b>%1</b>").arg(text);
}

QString BoldDecorator::removeBoldTags(const QString& text) {
    QString result = text;
    result.remove("<b>");
    result.remove("</b>");
    return result;
}
