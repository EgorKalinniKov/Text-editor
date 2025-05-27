#pragma once

#include <QString>
#include <memory>

class EditorContext;

// State interface
class IEditorState {
public:
    virtual ~IEditorState() = default;
    virtual void handleEdit(EditorContext* context, const QString& text) = 0;
    virtual QString getStateName() const = 0;
};

// Context
class EditorContext {
public:
    EditorContext();
    void setState(std::shared_ptr<IEditorState> state);
    void requestEdit(const QString& text);
    QString getStatus() const;

private:
    std::shared_ptr<IEditorState> state;
    QString status;

    friend class ReadOnlyState;
    friend class EditableState;
};

// Concrete States
class ReadOnlyState : public IEditorState {
public:
    void handleEdit(EditorContext* context, const QString& text) override;
    QString getStateName() const override { return "read-only"; }
};

class EditableState : public IEditorState {
public:
    void handleEdit(EditorContext* context, const QString& text) override;
    QString getStateName() const override { return "editable"; }
};