cmake_minimum_required(VERSION 3.14)

project(GUI LANGUAGES CXX)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(QT NAMES Qt5 COMPONENTS Core Quick REQUIRED)
find_package(Qt${QT_VERSION_MAJOR} COMPONENTS Core Quick REQUIRED) #if error run: sudo apt-get install libgl1-mesa-dev

set(PROJECT_SOURCES
        ../lib/Logger/logger.cpp
        ../lib/TCP/client/tcpClient.cpp
        ../lib/YAML/yaml.cpp
        ../Operations/Operations.cpp
        client.cpp
        backend.cpp
        main.cpp
        qml.qrc
)

add_executable(GUI ${PROJECT_SOURCES})

target_include_directories(GUI PRIVATE
        "../lib/TCP/client/"
        "../Operations/"
        )

target_compile_definitions(GUI PRIVATE $<$<OR:$<CONFIG:Debug>,$<CONFIG:RelWithDebInfo>>:QT_QML_DEBUG>)

target_link_libraries(GUI PRIVATE 
        Qt${QT_VERSION_MAJOR}::Core 
        Qt${QT_VERSION_MAJOR}::Quick
        yaml-cpp
        )