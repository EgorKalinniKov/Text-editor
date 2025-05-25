#ifndef TEXTDECORATOR_HPP
#define TEXTDECORATOR_HPP

#include <QTextEdit>

// Component
class TextComponent {
public:
    virtual ~TextComponent() = default;
    virtual QString getText() = 0;
    virtual void setText(const QString& text) = 0;
};

// Concrete Component
class SimpleTextEdit : public TextComponent {
public:
    SimpleTextEdit(QTextEdit* editor);
    QString getText() override;
    void setText(const QString& text) override;

private:
    QTextEdit* textEdit;
};

// Decorator
class TextDecorator : public TextComponent {
public:
    TextDecorator(std::shared_ptr<TextComponent> component);
    QString getText() override;
    void setText(const QString& text) override;

protected:
    std::shared_ptr<TextComponent> wrapped;
};

// Concrete Decorator A (Spell Check)
class SpellCheckDecorator : public TextDecorator {
public:
    SpellCheckDecorator(std::shared_ptr<TextComponent> component);
    QString getText() override;
    void setText(const QString& text) override;

private:
    QString checkSpelling(const QString& text);
};

#endif // TEXTDECORATOR_HPP