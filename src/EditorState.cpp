#include "EditorState.hpp"

EditorContext::EditorContext() : state(std::make_shared<ReadOnlyState>()) {}

void EditorContext::setState(std::shared_ptr<IEditorState> state) {
    this->state = state;
    this->status = state->getStateName();
}

void EditorContext::requestEdit(const QString& text) {
    if (state) {
        state->handleEdit(this, text);
    }
}

QString EditorContext::getStatus() const {
    return status;
}

void ReadOnlyState::handleEdit(EditorContext* context, const QString& text) {
    context->status = "Cannot edit in read-only mode";
}

void EditableState::handleEdit(EditorContext* context, const QString& text) {
    context->status = "Edit successful: " + text;
}