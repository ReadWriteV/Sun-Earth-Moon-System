cmake_minimum_required(VERSION 3.5.0)

project(SunEarthMoonSystem VERSION 1.0.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)

find_package(glad CONFIG REQUIRED)
find_package(glfw3 CONFIG REQUIRED)
find_package(Stb REQUIRED)

add_executable(${PROJECT_NAME} src/main.cpp src/sphere.cpp src/shader.cpp src/camera.cpp)
target_include_directories(${PROJECT_NAME} PRIVATE ${Stb_INCLUDE_DIR} ${CMAKE_CURRENT_SOURCE_DIR}/include)
target_link_libraries(${PROJECT_NAME} PRIVATE glad::glad glfw)
