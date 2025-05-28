cmake_minimum_required(VERSION 3.5)

project(TextEditorProject LANGUAGES CXX)

# Настройка стандарта C++
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Находим Qt5
find_package(Qt5 COMPONENTS Widgets Test REQUIRED)

# Включаем автогенерацию MOC файлов Qt
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

# Настройка для создания GUI приложения без консоли на Windows
if(WIN32)
    set(GUI_TYPE WIN32)
    
    # Получаем путь к директории с библиотеками Qt
    get_target_property(QT_QMAKE_EXECUTABLE Qt5::qmake IMPORTED_LOCATION)
    get_filename_component(QT_WINDEPLOYQT_EXECUTABLE ${QT_QMAKE_EXECUTABLE} PATH)
    set(QT_WINDEPLOYQT_EXECUTABLE "${QT_WINDEPLOYQT_EXECUTABLE}/windeployqt.exe")

    # Добавляем путь к OpenGL библиотекам
    if(MSVC)
        set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} /SUBSYSTEM:WINDOWS")
    endif()
endif()

include_directories(${CMAKE_SOURCE_DIR}/include)

set(SOURCES
    src/MainWindow.cpp
    src/TextDecorator.cpp
    src/DocumentAdapter.cpp
    src/FileHandler.cpp
    src/Command.cpp
    src/DocumentObserver.cpp
    src/TextIterator.cpp
    src/EditorState.cpp
    src/FileFacade.cpp
)

set(HEADERS
    include/MainWindow.hpp
    include/TextDecorator.hpp
    include/DocumentAdapter.hpp
    include/FileHandler.hpp
    include/Command.hpp
    include/DocumentObserver.hpp
    include/TextIterator.hpp
    include/EditorState.hpp
    include/FileFacade.hpp
)

# Создаем библиотеку из исходных файлов
add_library(TextEditorLib STATIC ${SOURCES} ${HEADERS})
target_include_directories(TextEditorLib PUBLIC ${CMAKE_SOURCE_DIR}/include)
target_link_libraries(TextEditorLib PUBLIC Qt5::Widgets)

# Создаем исполняемый файл
add_executable(TextEditorProject ${GUI_TYPE} src/main.cpp)
target_link_libraries(TextEditorProject PRIVATE TextEditorLib)

# Настраиваем развертывание для Windows
if(WIN32)
    add_custom_command(TARGET TextEditorProject POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E env PATH="${QT_WINDEPLOYQT_EXECUTABLE}/../;" ${QT_WINDEPLOYQT_EXECUTABLE} --no-opengl-sw --no-angle --no-system-d3d-compiler $<TARGET_FILE:TextEditorProject>
        COMMENT "Deploying Qt dependencies..."
    )
endif()

# Добавляем Google Test
include(FetchContent)
FetchContent_Declare(
    googletest
    URL https://github.com/google/googletest/archive/refs/tags/v1.14.0.zip
    DOWNLOAD_EXTRACT_TIMESTAMP TRUE
)
# Отключаем установку Google Test, так как он нужен только для тестирования
set(INSTALL_GTEST OFF CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

# Включаем тестирование
enable_testing()

# Добавляем тесты
add_subdirectory(tests)