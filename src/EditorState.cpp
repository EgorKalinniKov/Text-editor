#include "EditorState.hpp"

EditorContext::EditorContext() : state(std::make_shared<EditableState>()) {}

void EditorContext::setState(std::shared_ptr<EditorState> state) {
    this->state = state;
}

void EditorContext::requestEdit(const QString& text) {
    state->handleEdit(this, text);
}

QString EditorContext::getStatus() const {
    return status;
}

void ReadOnlyState::handleEdit(EditorContext* context, const QString& text) {
    context->getStatus() = "Cannot edit in read-only mode";
}

void EditableState::handleEdit(EditorContext* context, const QString& text) {
    context->getStatus() = "Edited: " + text;
}