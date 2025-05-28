cmake_minimum_required(VERSION 3.5)

# Находим Qt5
find_package(Qt5 COMPONENTS Core Widgets Test REQUIRED)

# Добавляем исполняемый файл для тестов
add_executable(editor_tests
    EditorStateTest.cpp
    DocumentAdapterTest.cpp
)

# Подключаем заголовочные файлы
target_include_directories(editor_tests PRIVATE
    ${CMAKE_SOURCE_DIR}/include
)

# Линкуем с необходимыми библиотеками
target_link_libraries(editor_tests PRIVATE
    Qt5::Core
    Qt5::Widgets
    Qt5::Test
    GTest::gtest
    GTest::gtest_main
    TextEditorLib
)

# Добавляем тест в систему тестирования CMake
include(GoogleTest)
gtest_discover_tests(editor_tests) 