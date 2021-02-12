cmake_minimum_required(VERSION 2.8)
project(hsl_colorizer)


find_package(glfw3 3.3 REQUIRED)

find_package(OpenGL REQUIRED)
find_package(glm REQUIRED)





add_library("glad" "glad/src/gl.c")
target_include_directories("glad" PRIVATE "glad/include")
#target_link_libraries(${PROJECT_NAME} "glad" "${CMAKE_DL_LIBS}")


add_executable(${PROJECT_NAME}_test main.cpp hsl_colorizer.hpp )
target_link_libraries(${PROJECT_NAME}_test glfw OpenGL::GL glad)
target_include_directories(${PROJECT_NAME}_test PRIVATE "glad/include")
