cmake_minimum_required(VERSION 3.7)

project(game-thing)
set(CMAKE_BUILD_TYPE Debug)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
find_package(SDL2 REQUIRED)
find_package(SDL2_image REQUIRED)
include_directories(${SDL2_INCLUDE_DIRS}  ${SDL2_IMAGE_INCLUDE_DIRS})

add_executable(game-thing src/Main.cpp src/sdl_window.cpp src/graphics.cpp src/renderer.cpp src/texture.cpp src/rect.cpp)
target_link_libraries(game-thing ${SDL2_LIBRARIES} ${SDL2_IMAGE_LIBRARIES})