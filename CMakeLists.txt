cmake_minimum_required(VERSION 3.23)
project(chess_renderer LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include(FetchContent)
FetchContent_Declare(
  SDL2
  GIT_REPOSITORY https://github.com/libsdl-org/SDL.git
  GIT_TAG        release-2.30.0
)
FetchContent_MakeAvailable(SDL2)

FetchContent_Declare(
  SDL2_image
  GIT_REPOSITORY https://github.com/libsdl-org/SDL_image.git
  GIT_TAG        release-2.8.2
)
FetchContent_MakeAvailable(SDL2_image)

add_executable(chess-renderer
    src/main.cpp
    src/game.cpp
    src/engine/board.cpp
    src/ui/renderer.cpp
    src/ui/mouseState.cpp
)

target_include_directories(chess-renderer PRIVATE src)

target_link_libraries(chess-renderer PRIVATE
    SDL2::SDL2main
    SDL2::SDL2
    SDL2_image::SDL2_image
)

add_custom_command(TARGET chess-renderer POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
            ${CMAKE_SOURCE_DIR}/assets
            $<TARGET_FILE_DIR:chess-renderer>/assets)