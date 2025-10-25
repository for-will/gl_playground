cmake_minimum_required(VERSION 4.0)
project(gl_playground)

set(CMAKE_CXX_STANDARD 17)

unset(GLTOOLS_LIBRARY CACHE)

find_package(X11)
find_package(OpenGL REQUIRED)
find_package(GLUT REQUIRED)
find_package(PkgConfig REQUIRED)
find_library(M_LIBRARY m)
find_library(GLTOOLS_LIBRARY gltools PATHS "${PROJECT_SOURCE_DIR}/GLTools/lib" NO_CACHE NO_DEFAULT_PATH)
pkg_search_module(GLEW REQUIRED glew)
pkg_search_module(GLFW REQUIRED glfw3)

MESSAGE(STATUS "GLEW_INCLUDE_DIRS: ${GLEW_INCLUDE_DIRS}")
MESSAGE(STATUS "GLEW_LIBRARY_DIRS: ${GLEW_LIBRARY_DIRS}")
MESSAGE(STATUS "GLFW_INCLUDE_DIRS: ${GLFW_INCLUDE_DIRS}")
MESSAGE(STATUS "GLFW_LIBRARY_DIRS: ${GLFW_LIBRARY_DIRS}")
MESSAGE(STATUS "GLTOOLS_LIBRARY: ${GLTOOLS_LIBRARY}")
MESSAGE(STATUS "GLEW_LIBRARIES: ${GLEW_LIBRARIES}")

include_directories(${GLEW_INCLUDE_DIRS} ${GLFW_INCLUDE_DIRS})
link_directories(${GLEW_LIBRARY_DIRS} ${GLFW_LIBRARY_DIRS})

include_directories("${PROJECT_SOURCE_DIR}/GLTools/include"
        "${PROJECT_SOURCE_DIR}/glad/include"
        "${PROJECT_SOURCE_DIR}/stb"
        "${PROJECT_SOURCE_DIR}"
)

add_executable(gl_hello main.cpp)

set(LIBS
        ${OPENGL_LIBRARIES}
        ${GLUT_LIBRARIES}
        ${GLEW_LIBRARIES}
        ${GLFW_LIBRARIES}
        ${M_LIBRARY}
        ${X11_LIBRARIES}
        ${GLTOOLS_LIBRARY}
)

add_executable(gl_triangle src/triangle.cpp)

target_link_libraries(gl_triangle ${LIBS})

add_executable(hello_glut src/hello_glut.cpp)

target_link_libraries(hello_glut ${LIBS})

set(hello_names
        hello_glad
        hello_triangle
        hello_shader
        hello_shader_class
        hello_texture
        hello_transformation
        hello_coordinate
        hello_camera
        hello_camera_move
        hello_lamp
)

foreach (name ${hello_names})
    add_executable(${name}
            src/${name}.cpp
            glad/src/glad.c
            stb/stb_image.cpp
    )

    target_link_libraries(${name}
            ${OPENGL_LIBRARIES}
            ${GLFW_LIBRARIES}
            ${M_LIBRARY}
            ${X11_LIBRARIES}
    )
endforeach ()

