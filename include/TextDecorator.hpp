#pragma once

#include <QTextEdit>
#include <QString>
#include <QColor>
#include <memory>

// Component interface
class TextComponent {
public:
    virtual ~TextComponent() = default;
    virtual QString getText() = 0;
    virtual void setText(const QString& text) = 0;
    virtual QString getFormattedText() = 0;  // Новый метод для получения форматированного текста
};

// Concrete Component
class SimpleTextEdit : public TextComponent {
public:
    explicit SimpleTextEdit(QTextEdit* editor);
    QString getText() override;
    void setText(const QString& text) override;
    QString getFormattedText() override;

private:
    QTextEdit* textEdit;
};

// Base Decorator
class TextDecorator : public TextComponent {
public:
    explicit TextDecorator(std::shared_ptr<TextComponent> component);
    QString getText() override;
    void setText(const QString& text) override;
    QString getFormattedText() override;

protected:
    std::shared_ptr<TextComponent> wrapped;
};

// Concrete Decorator A - Italic
class ItalicDecorator : public TextDecorator {
public:
    explicit ItalicDecorator(std::shared_ptr<TextComponent> component);
    QString getText() override;
    void setText(const QString& text) override;
    QString getFormattedText() override;

private:
    QString addItalicTags(const QString& text);
    QString removeItalicTags(const QString& text);
};

// Concrete Decorator B - Color
class ColorDecorator : public TextDecorator {
public:
    explicit ColorDecorator(std::shared_ptr<TextComponent> component, const QColor& color = Qt::blue);
    QString getText() override;
    void setText(const QString& text) override;
    QString getFormattedText() override;

private:
    QString addColorTags(const QString& text);
    QString removeColorTags(const QString& text);
    QColor textColor;
};

// Concrete Decorator C - Bold
class BoldDecorator : public TextDecorator {
public:
    explicit BoldDecorator(std::shared_ptr<TextComponent> component);
    QString getText() override;
    void setText(const QString& text) override;
    QString getFormattedText() override;

private:
    QString addBoldTags(const QString& text);
    QString removeBoldTags(const QString& text);
};