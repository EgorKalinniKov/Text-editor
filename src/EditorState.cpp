#include "EditorState.hpp"
#include <QDateTime>

EditorContext::EditorContext() : state(std::make_shared<SavedState>()) {}

void EditorContext::setState(std::shared_ptr<IEditorState> newState) {
    this->state = newState;
    this->status = state->getStateName();
    this->lastModified = QDateTime::currentDateTime();
}

void EditorContext::requestEdit(const QString& text) {
    if (state) {
        state->handleEdit(this, text);
    }
}

void EditorContext::requestSave() {
    if (state) {
        state->handleSave(this);
    }
}

QString EditorContext::getStatus() const {
    return status;
}

QDateTime EditorContext::getLastModified() const {
    return lastModified;
}

// Saved State
void SavedState::handleEdit(EditorContext* context, const QString& text) {
    context->content = text;
    context->setState(std::make_shared<ModifiedState>());
}

void SavedState::handleSave(EditorContext* context) {
    context->status = "Document already saved";
}

QString SavedState::getStateName() const {
    return "Saved";
}

// Modified State
void ModifiedState::handleEdit(EditorContext* context, const QString& text) {
    context->content = text;
    context->status = "Document modified";
}

void ModifiedState::handleSave(EditorContext* context) {
    // Here would be actual save logic
    context->setState(std::make_shared<SavedState>());
    context->status = "Document saved successfully";
}

QString ModifiedState::getStateName() const {
    return "Modified";
}

// AutoSaving State
void AutoSavingState::handleEdit(EditorContext* context, const QString& text) {
    context->content = text;
    if (shouldAutoSave()) {
        handleSave(context);
    } else {
        context->status = "Document modified (auto-save pending)";
    }
}

void AutoSavingState::handleSave(EditorContext* context) {
    // Here would be auto-save logic
    lastAutoSave = QDateTime::currentDateTime();
    context->setState(std::make_shared<SavedState>());
    context->status = "Document auto-saved";
}

QString AutoSavingState::getStateName() const {
    return "AutoSaving";
}

bool AutoSavingState::shouldAutoSave() const {
    return lastAutoSave.secsTo(QDateTime::currentDateTime()) >= autoSaveInterval;
}

// Error State
void ErrorState::handleEdit(EditorContext* context, const QString& text) {
    context->status = "Cannot edit: " + errorMessage;
}

void ErrorState::handleSave(EditorContext* context) {
    context->status = "Cannot save: " + errorMessage;
}

QString ErrorState::getStateName() const {
    return "Error";
}

ErrorState::ErrorState(const QString& message) : errorMessage(message) {}