cmake_minimum_required(VERSION 3.10)
project(OpenGlLearning VERSION 1.0.0)
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED True)
set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
add_subdirectory(${PROJECT_SOURCE_DIR}/../libs/glfw-3.3.8-source glfw)
add_executable(Start Part1.cpp glad.c  Program/shader.cpp common/gl_common.cpp 
common/TextureSample.cpp common/TextureSample3D.cpp)
target_include_directories(Start PUBLIC ${PROJECT_BINARY_DIR} 
    ${PROJECT_SOURCE_DIR}/../libs/glfw-3.3.8-source/include  ${PROJECT_SOURCE_DIR}/../include)
target_link_libraries(Start PUBLIC glfw)
find_package(OpenGL REQUIRED)
target_link_libraries(Start PUBLIC OpenGL::GL)

