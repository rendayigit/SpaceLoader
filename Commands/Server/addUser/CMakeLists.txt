project(addUser)
cmake_minimum_required(VERSION 3.2.0)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(QT NAMES Qt5 COMPONENTS Core Quick REQUIRED)
find_package(Qt${QT_VERSION_MAJOR} COMPONENTS Core Quick REQUIRED) # if error run: sudo apt-get install libgl1-mesa-dev
find_package(Qt5Widgets REQUIRED)

set(COMPILE_OPTIONS -std=c++11)

include_directories(${PROJECT_SOURCE_DIR}/../../../)
include_directories(${PROJECT_SOURCE_DIR}/../../)
include_directories(${PROJECT_SOURCE_DIR}/../)

add_library(addUser SHARED main.cpp
                           ../../../lib/TCP/server/tcpServer.cpp
                           ../../../lib/Logger/logger.cpp
                           )
set_target_properties(addUser PROPERTIES
         LIBRARY_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/../../../bin)

target_compile_definitions(addUser PRIVATE $<$<OR:$<CONFIG:Debug>,$<CONFIG:RelWithDebInfo>>:QT_QML_DEBUG>)

target_link_libraries(addUser
        PRIVATE Qt${QT_VERSION_MAJOR}::Core
        Qt${QT_VERSION_MAJOR}::Quick
        )