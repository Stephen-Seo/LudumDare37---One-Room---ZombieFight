cmake_minimum_required(VERSION 3.0)

project(LudumDare37_OneRoom)

set(LD37_SOURCES
    src/Main.cpp
    src/Game.cpp
    src/RoomScreen.cpp
    src/Sprite.cpp
    src/PathFinding.cpp
)

include_directories(${CMAKE_CURRENT_SOURCE_DIR}/src)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++14 -Wall -Wextra -Wpedantic")
set(CMAKE_CXX_FLAGS_DEBUG "-O0 -g")
set(CMAKE_CXX_FLAGS_RELEASE "-O3 -D NDEBUG")

if(NOT CMAKE_BUILD_TYPE AND NOT CMAKE_CONFIGURATION_TYPES)
    message(STATUS "Setting build type to 'Debug', none was specified.")
    set(CMAKE_BUILD_TYPE Debug CACHE STRING "Choose the type of build." FORCE)
    set_property(CACHE CMAKE_BUILD_TYPE PROPERTY STRINGS "Debug" "Release")
endif()

add_executable(LD37 ${LD37_SOURCES})

find_package(SFML 2 REQUIRED audio graphics window system)
target_include_directories(LD37 PUBLIC ${SFML_INCLUDE_DIR})
target_link_libraries(LD37 PUBLIC ${SFML_LIBRARIES})

add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/GameDevTools)
target_include_directories(LD37 PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/GameDevTools/src)
target_link_libraries(LD37 PUBLIC GameDevTools)

