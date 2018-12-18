cmake_minimum_required(VERSION 3.9)
project(VoxelViewer)

set(CMAKE_CXX_STANDARD 11)
set(SOURCE_FILES main.cpp)
set(USER_INCLUDE_DIR include/)

add_executable(VoxelViewer ${SOURCE_FILES})

find_package(OpenGL REQUIRED)
find_package(GLUT REQUIRED)

include_directories(${OPENGL_INCLUDE_DIRS} ${GLUT_INCLUDE_DIRS} ${USER_INCLUDE_DIR})
target_link_libraries(${PROJECT_NAME} ${OPENGL_LIBRARY} ${GLUT_LIBRARY})
