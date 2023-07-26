cmake_minimum_required(VERSION 3.14)

project(SpaceLoader VERSION 1.0 DESCRIPTION "SpaceLoader")

enable_testing()
include(GoogleTest)

add_subdirectory(Server) # Console Server Application
add_subdirectory(Client_console) # Console Client Application
add_subdirectory(Client_gui) # Client Application With Graphical User Interface
add_subdirectory(Commands) # Dynamic libraries for all commands
add_subdirectory(tests/) # Unit tests