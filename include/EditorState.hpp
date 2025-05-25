#ifndef EDITORSTATE_HPP
#define EDITORSTATE_HPP

#include <QString>
#include <memory>

class EditorContext;

// State interface
class EditorState {
public:
    virtual ~EditorState() = default;
    virtual void handleEdit(EditorContext* context, const QString& text) = 0;
};

// Context
class EditorContext {
public:
    EditorContext();
    void setState(std::shared_ptr<EditorState> state);
    void requestEdit(const QString& text);
    QString getStatus() const;

private:
    std::shared_ptr<EditorState> state;
    QString status;
};

// Concrete States
class ReadOnlyState : public EditorState {
public:
    void handleEdit(EditorContext* context, const QString& text) override;
};

class EditableState : public EditorState {
public:
    void handleEdit(EditorContext* context, const QString& text) override;
};

#endif // EDITORSTATE_HPP