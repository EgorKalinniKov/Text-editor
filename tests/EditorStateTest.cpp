#include <gtest/gtest.h>
#include "EditorState.hpp"

class EditorStateTest : public ::testing::Test {
protected:
    void SetUp() override {
        context = std::make_shared<EditorContext>();
    }

    std::shared_ptr<EditorContext> context;
};

// Тест перехода между состояниями
TEST_F(EditorStateTest, StateTransitions) {
    // Начальное состояние должно быть Saved
    EXPECT_EQ(context->getStatus(), "Saved");
    
    // Переход в состояние Modified
    context->setState(std::make_shared<ModifiedState>());
    EXPECT_EQ(context->getStatus(), "Modified");
    
    // Переход обратно в состояние Saved
    context->setState(std::make_shared<SavedState>());
    EXPECT_EQ(context->getStatus(), "Saved");
}

// Тест обработки редактирования в разных состояниях
TEST_F(EditorStateTest, EditHandling) {
    // В состоянии Saved редактирование должно переводить в Modified
    context->setState(std::make_shared<SavedState>());
    context->requestEdit("test");
    EXPECT_EQ(context->getStatus(), "Modified");
    
    // В состоянии Modified редактирование должно оставлять статус "Document modified"
    context->requestEdit("test again");
    EXPECT_EQ(context->getStatus(), "Document modified");
}

// Тест обработки сохранения
TEST_F(EditorStateTest, SaveHandling) {
    // Переводим в состояние Modified
    context->setState(std::make_shared<ModifiedState>());
    EXPECT_EQ(context->getStatus(), "Modified");
    
    // Сохранение должно установить статус "Document saved successfully"
    context->requestSave();
    EXPECT_EQ(context->getStatus(), "Document saved successfully");
}

// Тест состояния ошибки
TEST_F(EditorStateTest, ErrorState) {
    QString errorMessage = "Test error";
    context->setState(std::make_shared<ErrorState>(errorMessage));
    
    // Проверяем, что состояние содержит сообщение об ошибке
    EXPECT_EQ(context->getStatus(), "Error: " + errorMessage);
    
    // Проверяем, что в состоянии ошибки нельзя редактировать
    context->requestEdit("test");
    EXPECT_EQ(context->getStatus(), "Cannot edit: " + errorMessage);
}

// Тест автосохранения
TEST_F(EditorStateTest, AutoSaving) {
    auto autoSaveState = std::make_shared<AutoSavingState>();
    context->setState(autoSaveState);
    
    // Проверяем начальное состояние
    EXPECT_EQ(context->getStatus(), "AutoSaving");
    
    // Редактирование должно вызывать попытку автосохранения
    context->requestEdit("test");
    QString status = context->getStatus();
    EXPECT_TRUE(status == "Document modified (auto-save pending)" || 
                status == "Document auto-saved");
} 