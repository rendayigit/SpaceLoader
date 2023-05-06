cmake_minimum_required(VERSION 3.14)

project(CONSOLE LANGUAGES CXX)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/../bin/)

find_package(QT NAMES Qt5 COMPONENTS Core Quick REQUIRED)
find_package(Qt${QT_VERSION_MAJOR} COMPONENTS Core Quick REQUIRED) # if error run: sudo apt-get install libgl1-mesa-dev

set(PROJECT_SOURCES
        # ../lib/XML/xml.cpp
        ../lib/Logger/logger.cpp
        ../lib/TCP/client/tcpClient.cpp
        ../lib/YAML/yaml.cpp
        ../Common/CommonClientServer.cpp
        ../Common/CommonClient.cpp
        ClientConsole.cpp
        main.cpp
        )

add_executable(CONSOLE ${PROJECT_SOURCES})

target_include_directories(CONSOLE PRIVATE
        "../lib/TCP/client/"
        "../CommonClientServer/"
        )

target_compile_definitions(CONSOLE PRIVATE $<$<OR:$<CONFIG:Debug>,$<CONFIG:RelWithDebInfo>>:QT_QML_DEBUG>)

target_link_libraries(CONSOLE
        PRIVATE Qt${QT_VERSION_MAJOR}::Core
        Qt${QT_VERSION_MAJOR}::Quick
        yaml-cpp
        stdc++fs
        )