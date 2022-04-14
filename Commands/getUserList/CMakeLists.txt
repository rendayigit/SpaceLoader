project(getUserList)
cmake_minimum_required(VERSION 3.2.0)

set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_AUTOMOC ON)

find_package(Qt5Widgets REQUIRED)

set(COMPILE_OPTIONS -std=c++11)

add_library(getUserList SHARED test-plugin.cpp)

find_package(Qt5 COMPONENTS Network REQUIRED)

set_target_properties(getUserList PROPERTIES
         LIBRARY_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/../../Libs/)

target_link_libraries(getUserList PRIVATE Qt5::Widgets Qt5::Network)

target_compile_options(getUserList PRIVATE ${COMPILE_OPTIONS})

set_target_properties(getUserList PROPERTIES
  IMPORTED_LOCATION "${CMAKE_SOURCE_DIR}/libs/libgetUserList.so"
  INTERFACE_INCLUDE_DIRECTORIES "${CMAKE_SOURCE_DIR}/libs/libgetUserList"
)
