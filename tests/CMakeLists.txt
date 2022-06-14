cmake_minimum_required(VERSION 3.15)

project(tests LANGUAGES CXX)

enable_testing()

set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(QT NAMES Qt5 COMPONENTS Core Quick Qt5XmlPatterns REQUIRED)
find_package(Qt${QT_VERSION_MAJOR} COMPONENTS Core Quick REQUIRED) # if error run: sudo apt-get install libgl1-mesa-dev
find_package(Qt5Widgets REQUIRED)

add_executable(tests
        ../lib/Logger/logger.cpp
        Logger/test_logger.cpp
        YAML/test_yaml.cpp
        ../lib/YAML/yaml.cpp
        ../lib/TCP/client/tcpClient.cpp
        ../lib/TCP/server/tcpServer.cpp
        TCP/testClient.cpp
        TCP/testServer.cpp
        TCP/test_tcp.cpp
        Commands/test_commands.cpp
        ../Commands/hello/hello.cpp
)   

target_compile_definitions(tests PRIVATE $<$<OR:$<CONFIG:Debug>,$<CONFIG:RelWithDebInfo>>:QT_QML_DEBUG>)

target_link_libraries(tests PRIVATE 
        Qt${QT_VERSION_MAJOR}::Core
        Qt${QT_VERSION_MAJOR}::Quick
        gtest_main
        gtest
        yaml-cpp
        -lpthread 
        -lm
        )

include(GoogleTest)
# gtest_discover_tests(tests)