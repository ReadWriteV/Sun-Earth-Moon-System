cmake_minimum_required(VERSION 3.19)

project(Blinn-Phong)

set(CMAKE_BUILD_TYPE "Release")

include_directories(include)
include_directories(path/to/glfw/include)
include_directories(path/to/glew/include)
include_directories(path/to/glm)
include_directories(path/to/SOIL2/include)

add_definitions(-DGLEW_STATIC)

link_directories(path/to/glfw/lib-vc2019)
link_directories(path/to/glew/lib/Release/x64)
link_directories(path/to/SOIL2/lib)

add_executable(${PROJECT_NAME} src/main.cpp src/sphere.cpp)

target_link_libraries(${PROJECT_NAME} glfw3 glew32s opengl32 soil2)