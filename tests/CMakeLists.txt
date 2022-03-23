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

if (DEFINED ENV{GITHUB_ACTIONS})
    link_directories("D:/gtest/lib")
    include_directories("D:/gtest/include")
    link_directories("D:/yaml-cpp/lib")
    include_directories("D:/yaml-cpp/include")

    set(CMAKE_CXX_FLAGS_RELEASE "/MT")
    set(CMAKE_CXX_FLAGS_DEBUG "/MTd")
endif ()

add_executable(tests
                ../lib/Logger/logger.cpp
                Logger/test_logger.cpp
                # YAML/test_yaml.cpp
                # ../lib/YAML/yaml.cpp
                # ../lib/TCP/client/tcpClient.cpp
                # ../lib/TCP/server/tcpServer.cpp
                # TCP/testClient.cpp
                # TCP/testServer.cpp
                # TCP/test_tcp.cpp
)   

target_compile_definitions(tests PRIVATE $<$<OR:$<CONFIG:Debug>,$<CONFIG:RelWithDebInfo>>:QT_QML_DEBUG>)

target_link_libraries(tests
        PRIVATE Qt${QT_VERSION_MAJOR}::Core
        Qt${QT_VERSION_MAJOR}::Quick
        gtest_main
        gtest
        yaml-cpp
        Qt5::Core
        Qt5::Quick
        )

if (UNIX AND NOT APPLE) # FOR LINUX ONLY
        target_link_libraries(tests PRIVATE 
        -lpthread 
        -lm
        )
endif ()

include(GoogleTest)
gtest_discover_tests(tests)