cmake_minimum_required(VERSION 3.26)
project(PlatformerPrototype LANGUAGES CXX)

# ---------------- Output ----------------
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

# ---------------- C++ Standard ----------------
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# ---------------- SFML (FetchContent) ----------------
include(FetchContent)

FetchContent_Declare(
    SFML
    GIT_REPOSITORY https://github.com/SFML/SFML.git
    GIT_TAG 3.0.2
    GIT_SHALLOW ON
)

FetchContent_MakeAvailable(SFML)

# ---------------- Executable ----------------
add_executable(PlatformerPrototype
    src/main.cpp
    src/Game.cpp
    src/Player.cpp
    src/Enemy.cpp
    src/Level.cpp
)

# ---------------- Includes ----------------
target_include_directories(PlatformerPrototype PRIVATE src)

# ---------------- Link ----------------
target_link_libraries(PlatformerPrototype
    PRIVATE
        SFML::Graphics
        SFML::Window
        SFML::System
)

# ---------------- Warnings ----------------
if (MSVC)
    target_compile_options(PlatformerPrototype PRIVATE /W4)
else()
    target_compile_options(PlatformerPrototype PRIVATE -Wall -Wextra -Wpedantic)
endif()
