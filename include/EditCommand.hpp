#ifndef EDITCOMMAND_HPP
#define EDITCOMMAND_HPP

#include <QString>
#include <memory>

// Command interface
class Command {
public:
    virtual ~Command() = default;
    virtual void execute() = 0;
    virtual void undo() = 0;
};

// Receiver
class TextReceiver {
public:
    TextReceiver(QString& text);
    void setText(const QString& newText);
    QString getText() const;

private:
    QString& textContent;
};

// Concrete Command
class EditTextCommand : public Command {
public:
    EditTextCommand(std::shared_ptr<TextReceiver> receiver, const QString& newText, const QString& oldText);
    void execute() override;
    void undo() override;

private:
    std::shared_ptr<TextReceiver> receiver;
    QString newText;
    QString oldText;
};

#endif // EDITCOMMAND_HPP