cmake_minimum_required(VERSION 2.8)
project(raytracer)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -O3 -fopenmp")
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_CURRENT_LIST_DIR}")

set(SOURCE_FILES
    src/color.h
    src/compiler_opt.h
    src/aabb.cpp
    src/aabb.h
    src/image_writer.cpp
    src/image_writer.h
    src/main.cpp
    src/material.h
    src/matrix.h
    src/mesh.cpp
    src/mesh.h
    src/platform.h
    src/ray.h
    src/raytracing.cpp
    src/raytracing.h
    src/scene.cpp
    src/scene.h
    src/scene_node.cpp
    src/scene_node.h
    src/vector4.h
    src/sphere.cpp
    src/sphere.h
    src/trackball.h
    src/trackball.cpp
    src/vector3.h
    src/vertex.h
    src/testray.h
    src/testray.cpp
    src/Triangle.h
    src/KDNode.cpp
    src/KDNode.h
)

add_executable(raytracer ${SOURCE_FILES})

#########################################################
# FIND GLUT
#########################################################
find_package(GLUT REQUIRED)
include_directories(${GLUT_INCLUDE_DIRS})
link_directories(${GLUT_LIBRARY_DIRS})
add_definitions(${GLUT_DEFINITIONS})
if(NOT GLUT_FOUND)
    message(ERROR " GLUT not found!")
endif(NOT GLUT_FOUND)
#########################################################
# FIND OPENGL
#########################################################
find_package(OpenGL REQUIRED)
include_directories(${OpenGL_INCLUDE_DIRS})
link_directories(${OpenGL_LIBRARY_DIRS})
add_definitions(${OpenGL_DEFINITIONS})
if(NOT OPENGL_FOUND)
    message(ERROR " OPENGL not found!")
endif(NOT OPENGL_FOUND)

#########################################################
# Include Files
#########################################################
target_link_libraries(raytracer ${OPENGL_LIBRARIES} ${GLUT_LIBRARY} )

