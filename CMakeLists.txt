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
    src/FileHandler.cpp
    src/DocumentAdapter.cpp
    src/TextDecorator.cpp
    src/EditCommand.cpp
    src/DocumentObserver.cpp
    src/TextIterator.cpp
    src/FileFacade.cpp
    src/FileProxy.cpp
    src/EditorState.cpp
    src/MainWindow.cpp
)

set(HEADERS
    include/FileHandler.hpp
    include/DocumentAdapter.hpp
    include/TextDecorator.hpp
    include/EditCommand.hpp
    include/DocumentObserver.hpp
    include/TextIterator.hpp
    include/FileFacade.hpp
    include/FileProxy.hpp
    include/EditorState.hpp
    include/MainWindow.hpp
)

add_executable(TextEditorProject ${SOURCES} ${HEADERS})

target_link_libraries(TextEditorProject Qt5::Widgets)