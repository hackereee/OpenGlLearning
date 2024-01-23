cmake_minimum_required(VERSION 3.10)
project(OpenGlLearning VERSION 1.0.0)
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED True)
configure_file(${PROJECT_SOURCE_DIR}/config/config.h.in config.h)
option(MACOS 'macos' ON)
set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
add_subdirectory(${PROJECT_SOURCE_DIR}/../libs/glfw-3.3.8-source glfw)
add_executable(Start 
main.cxx
practice/Part1.cpp 
glad.c  
Program/shader.cpp common/gl_common.cpp 
common/TextureSample.cpp 
common/TextureSample3D.cpp)
target_include_directories(Start PUBLIC ${PROJECT_BINARY_DIR} 
    ${PROJECT_SOURCE_DIR}/../libs/glfw-3.3.8-source/include  ${PROJECT_SOURCE_DIR}/../include)
set(RESOURCE_FILES ../resources/container.jpg ../resources/guoguo.jpg)
set(SHADER_FILES ../shaders/texture-3d.frag ../shaders/texture-3d.vert)
set_target_properties(Start
 PROPERTIES  
MACOSX_BUNDLE TRUE
MACOSX_FRAMEWORK_IDENTIFIER 
cc.hale.Start 
RESOURCE "${RESOURCE_FILES}"
RESOURCE "${SHADER_FILES}")
target_link_libraries(Start PUBLIC glfw)
find_package(OpenGL REQUIRED)
target_link_libraries(Start PUBLIC OpenGL::GL)

