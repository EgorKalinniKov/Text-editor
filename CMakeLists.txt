cmake_minimum_required(VERSION 3.5)

project(TextEditorProject LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)

find_package(Qt5 COMPONENTS Widgets REQUIRED)

include_directories(${CMAKE_SOURCE_DIR}/include)

set(SOURCES
    src/main.cpp
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

add_executable(TextEditorProject ${SOURCES} ${HEADERS})

target_link_libraries(TextEditorProject Qt5::Widgets)