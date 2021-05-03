cmake_minimum_required(VERSION 3.19)

project(test)

set(CMAKE_BUILD_TYPE "Release")

include_directories(include)
include_directories(D:/SDK/glfw-3.3.4/include)
include_directories(D:/SDK/glew-2.2.0/include)
include_directories(D:/SDK/glm-0.9.9.8)
include_directories(D:/SDK/SOIL2/include)

add_definitions(-DGLEW_STATIC)

link_directories(D:/SDK/glfw-3.3.4/lib-vc2019)
link_directories(D:/SDK/glew-2.2.0/lib/Release/x64)
link_directories(D:/SDK/SOIL2/lib)

add_executable(test src/main.cpp src/sphere.cpp)

target_link_libraries(test glfw3 glew32s opengl32 soil2)