cmake_minimum_required(VERSION 3.23)
project(chess_renderer LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# ── SDL2 pulled at configure-time ────────────────────────────────
include(FetchContent)
FetchContent_Declare(
  SDL2
  GIT_REPOSITORY https://github.com/libsdl-org/SDL.git
  GIT_TAG        release-2.30.0
)
FetchContent_MakeAvailable(SDL2)

# ── build the executable ─────────────────────────────────────────
add_executable(chess-renderer
    src/main.cpp                  # <- keep main at top level of src
    src/ui/renderer.cpp
)

# Allow #include "renderer.h" from anywhere inside src/
target_include_directories(chess-renderer PRIVATE src)

# Link SDL2 (handles frameworks / DLLs on each OS)
target_link_libraries(chess-renderer PRIVATE SDL2::SDL2main SDL2::SDL2)

# ── copy assets next to the output binary ────────────────────────
add_custom_command(TARGET chess-renderer POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
            ${CMAKE_SOURCE_DIR}/src/ui/assets          # ← source path
            $<TARGET_FILE_DIR:chess-renderer>/assets)  # ← destination
