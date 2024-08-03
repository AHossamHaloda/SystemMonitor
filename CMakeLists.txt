# Minimum CMake version required
cmake_minimum_required(VERSION 3.10)

# Project name
project(SystemMonitor)

# Set the C++ standard
set(CMAKE_CXX_STANDARD 17)

# Define common sources
file(GLOB_RECURSE COMMON_SOURCES "1_Software/main.cpp" "1_Software/Common/src/*.cpp")

# Initialize SOURCES
set(SOURCES ${COMMON_SOURCES})

# Create the executable
add_executable(monitor ${SOURCES})

# Include common headers directory
target_include_directories(monitor PRIVATE
    "1_Software/Common/inc"
)

# Define macros and platform-specific include directories based on BUILD_PLATFORM
if (BUILD_PLATFORM STREQUAL "linux")
    file(GLOB LINUX_SOURCES "1_Software/Linux/src/*.cpp")
    list(APPEND SOURCES ${LINUX_SOURCES})
    target_include_directories(monitor PRIVATE "1_Software/Linux/inc")
    target_compile_definitions(monitor PRIVATE LINUX_PLT)
elseif (BUILD_PLATFORM STREQUAL "macos")
    file(GLOB MACOS_SOURCES "1_Software/MacOS/src/*.cpp")
    list(APPEND SOURCES ${MACOS_SOURCES})
    target_include_directories(monitor PRIVATE "1_Software/MacOS/inc")
    target_compile_definitions(monitor PRIVATE MAC_OS_PLT)
else()
    message(FATAL_ERROR "Unsupported BUILD_PLATFORM value: ${BUILD_PLATFORM}")
endif()

# Update the executable with additional sources if any
target_sources(monitor PRIVATE ${SOURCES})

# Find and link the ncurses library
set(CURSES_NEED_NCURSES TRUE)
find_package(Curses REQUIRED)
target_include_directories(monitor PRIVATE ${CURSES_INCLUDE_DIRS})
target_link_libraries(monitor ${CURSES_LIBRARIES})
