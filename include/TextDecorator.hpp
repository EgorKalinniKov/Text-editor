#pragma once

#include <QTextEdit>
#include <QString>
#include <memory>
#include <QSet>

// Component interface
class TextComponent {
public:
    virtual ~TextComponent() = default;
    virtual QString getText() = 0;
    virtual void setText(const QString& text) = 0;
};

// Concrete Component
class SimpleTextEdit : public TextComponent {
public:
    explicit SimpleTextEdit(QTextEdit* editor);
    QString getText() override;
    void setText(const QString& text) override;

private:
    QTextEdit* textEdit;
};

// Base Decorator
class TextDecorator : public TextComponent {
public:
    explicit TextDecorator(std::shared_ptr<TextComponent> component);
    QString getText() override;
    void setText(const QString& text) override;

protected:
    std::shared_ptr<TextComponent> wrapped;
};

// Concrete Decorator A
class SpellCheckDecorator : public TextDecorator {
public:
    explicit SpellCheckDecorator(std::shared_ptr<TextComponent> component);
    QString getText() override;
    void setText(const QString& text) override;

private:
    QString spellCheck(const QString& text);
    QSet<QString> loadDictionary();
    QSet<QString> dictionary;
};

// Concrete Decorator B
class GrammarCheckDecorator : public TextDecorator {
public:
    explicit GrammarCheckDecorator(std::shared_ptr<TextComponent> component);
    QString getText() override;
    void setText(const QString& text) override;

private:
    QString checkGrammar(const QString& text);
};