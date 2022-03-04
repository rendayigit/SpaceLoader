cmake_minimum_required(VERSION 3.14)

project(SERVER LANGUAGES CXX)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

if (CMAKE_BUILD_TYPE EQUAL "DEBUG")
    message("debug mode")
elseif(CMAKE_BUILD_TYPE EQUAL "RELEASE")
    message("release mode")
    #TODO - Copy executable and required qt dlls to a deployment dir
endif (CMAKE_BUILD_TYPE EQUAL "DEBUG")

find_package(QT NAMES Qt5 COMPONENTS Core Quick REQUIRED)
find_package(Qt${QT_VERSION_MAJOR} COMPONENTS Core Quick REQUIRED) # if error run: sudo apt-get install libgl1-mesa-dev

set(PROJECT_SOURCES
        ../lib/TCP/client/tcpClient.cpp
        ../lib/YAML/yaml.cpp
        ../lib/Logger/logger.cpp
        ../lib/TCP/server/tcpServer.cpp
        ../Operations/Operations.cpp
        server.cpp
        main.cpp
        )

add_executable(SERVER ${PROJECT_SOURCES})

# TODO - check if this still works after the Setup/ files are updated
add_custom_command(
        TARGET SERVER POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_directory
            ${CMAKE_SOURCE_DIR}/Setup/
            ${CMAKE_CURRENT_BINARY_DIR}/Setup/)

target_include_directories(SERVER PRIVATE
        "../lib/TCP/server/"
        "../Operations/"
        )

target_compile_definitions(SERVER PRIVATE $<$<OR:$<CONFIG:Debug>,$<CONFIG:RelWithDebInfo>>:QT_QML_DEBUG>)

target_link_libraries(SERVER
        PRIVATE Qt${QT_VERSION_MAJOR}::Core
        Qt${QT_VERSION_MAJOR}::Quick
        yaml-cpp
        )
if (UNIX AND NOT APPLE) # FOR LINUX ONLY
        target_link_libraries(SERVER PRIVATE 
        stdc++fs
        )
endif ()