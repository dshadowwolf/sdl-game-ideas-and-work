cmake_minimum_required(VERSION 3.7)

project(game-thing)
set(CMAKE_BUILD_TYPE Debug)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
find_package(SDL2 REQUIRED)
find_package(SDL2_image REQUIRED)
find_package(SDL2TTF REQUIRED)
include_directories(${SDL2_INCLUDE_DIRS}  ${SDL2_IMAGE_INCLUDE_DIRS} ${SDL2TTF_INCLUDE_DIR})

add_executable(game-thing src/Main.cpp src/sdl_window.cpp src/graphics.cpp src/renderer.cpp src/texture.cpp src/rect.cpp src/panel.cpp)
target_link_libraries(game-thing ${SDL2_LIBRARIES} ${SDL2_IMAGE_LIBRARIES} ${SDL2TTF_LIBRARY})