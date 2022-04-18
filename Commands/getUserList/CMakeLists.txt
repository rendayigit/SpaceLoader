cmake_minimum_required(VERSION 3.14)
project(getUserList LANGUAGES CXX)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(QT NAMES Qt5 COMPONENTS Core Quick REQUIRED)
find_package(Qt${QT_VERSION_MAJOR} COMPONENTS Core Quick REQUIRED) # if error run: sudo apt-get install libgl1-mesa-dev

set(COMPILE_OPTIONS -std=c++17)

include_directories(${PROJECT_SOURCE_DIR}/../../)
include_directories(${PROJECT_SOURCE_DIR}/../)

add_library(getUserList SHARED 
        getUserList.cpp
        ../../../lib/TCP/server/tcpServer.cpp)

set_target_properties(getUserList PROPERTIES LIBRARY_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/../../bin/cmds/) # UNIX
set_target_properties(getUserList PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/../../bin/cmds/) # WINDOWS

target_compile_definitions(getUserList PRIVATE $<$<OR:$<CONFIG:Debug>,$<CONFIG:RelWithDebInfo>>:QT_QML_DEBUG>)

target_link_libraries(getUserList
        PRIVATE Qt${QT_VERSION_MAJOR}::Core
        Qt${QT_VERSION_MAJOR}::Quick
        )

target_compile_options(getUserList PRIVATE ${COMPILE_OPTIONS})