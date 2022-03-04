cmake_minimum_required(VERSION 3.15)

set(GTEST_PROJECT gtestProject)

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
find_package(GTest REQUIRED)
find_package(Qt5 COMPONENTS Core Xml REQUIRED)

add_executable(XML
                ../../lib/XML/xml.cpp
                test_xml.cpp                  
)   


target_link_libraries(XML
        PRIVATE Qt${QT_VERSION_MAJOR}::Core
        Qt${QT_VERSION_MAJOR}::Quick
        gtest_main
        gtest
        yaml-cpp
        Qt5::Core
        Qt5::Quick
        Qt5::Xml
)

if (UNIX AND NOT APPLE) # FOR LINUX ONLY
        target_link_libraries(XML PRIVATE 
        -lpthread 
        -lm
        )
endif ()

include(GoogleTest)
gtest_discover_tests(XML)
add_test(${GTEST_PROJECT} ${GTEST_PROJECT})