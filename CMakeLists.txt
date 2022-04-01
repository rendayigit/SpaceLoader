cmake_minimum_required(VERSION 3.14)

project(SpaceLoader VERSION 0.1 DESCRIPTION "SpaceLoader")

# enable_testing()

# set(CMAKE_CXX_STANDARD 17)
# set(CMAKE_CXX_STANDARD_REQUIRED ON)
# set(CMAKE_CXX_EXTENSIONS OFF)

add_subdirectory(Server)
# add_subdirectory(Client_console)

# add_subdirectory(Commands) # Dynamic libraries for all commands

# add_subdirectory(tests/) # unit tests