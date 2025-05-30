#pragma once

#include <QString>
#include <QDateTime>
#include <memory>
#include <vector>

class EditorContext;

// Abstract State
class IEditorState {
public:
    virtual ~IEditorState() = default;
    virtual void handleEdit(EditorContext* context, const QString& text) = 0;
    virtual void handleSave(EditorContext* context) = 0;
    virtual QString getStateName() const = 0;
    
protected:
    QString getTimestamp() const;
};

// State Observer Interface
class IStateObserver {
public:
    virtual ~IStateObserver() = default;
    virtual void onStateChanged(const QString& newState) = 0;
};

// Context
class EditorContext {
public:
    EditorContext();
    
    void setState(std::shared_ptr<IEditorState> state);
    void requestEdit(const QString& text);
    void requestSave();
    QString getStatus() const;
    QDateTime getLastModified() const;
    
    void addStateObserver(std::shared_ptr<IStateObserver> observer);
    void notifyStateChanged();

    friend class SavedState;
    friend class ModifiedState;
    friend class AutoSavingState;
    friend class ErrorState;

private:
    std::shared_ptr<IEditorState> state;
    std::shared_ptr<IEditorState> previousState;
    QString status;
    QString content;
    QDateTime lastModified;
    std::vector<std::shared_ptr<IStateObserver>> observers;
};

// Concrete States
class SavedState : public IEditorState {
public:
    void handleEdit(EditorContext* context, const QString& text) override;
    void handleSave(EditorContext* context) override;
    QString getStateName() const override;
};

class ModifiedState : public IEditorState {
public:
    void handleEdit(EditorContext* context, const QString& text) override;
    void handleSave(EditorContext* context) override;
    QString getStateName() const override;
};

class AutoSavingState : public IEditorState {
public:
    void handleEdit(EditorContext* context, const QString& text) override;
    void handleSave(EditorContext* context) override;
    QString getStateName() const override;

private:
    bool shouldAutoSave() const;
    QDateTime lastAutoSave;
    const int autoSaveInterval = 300; // 5 minutes in seconds
};

class ErrorState : public IEditorState {
public:
    explicit ErrorState(const QString& message);
    void handleEdit(EditorContext* context, const QString& text) override;
    void handleSave(EditorContext* context) override;
    QString getStateName() const override;

private:
    QString errorMessage;
};