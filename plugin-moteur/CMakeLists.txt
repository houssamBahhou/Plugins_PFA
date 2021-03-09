cmake_minimum_required(VERSION 3.16)
project(pfa_vite)

set(CMAKE_CXX_STANDARD 17)

find_package(OpenGL REQUIRED)
find_package(Vulkan)


find_package(Qt5 COMPONENTS Widgets Core REQUIRED)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC_SEARCH_PATHS src/opengl)

include_directories(src/opengl)

set(PROJECT_SOURCE_DIR ${PROJECT_SOURCE_DIR}/src)
set(SRC_COMMON ${PROJECT_SOURCE_DIR}/main.cpp)
set(SRC_DIR_VULKAN ${PROJECT_SOURCE_DIR}/vulkan)
set(SRC_DIR_OPENGL ${PROJECT_SOURCE_DIR}/opengl)

#RULE FOR VULKAN
set(SRC_VULKAN
        ${SRC_DIR_VULKAN}/vulkanrenderer.cpp
        ${SRC_DIR_VULKAN}/vulkanwindow.cpp
        )

add_executable(engine_vulkan ${SRC_COMMON} ${SRC_VULKAN} ${Vulkan_LIBRARY} ${QT_LIBRARIES})
qt5_use_modules(engine_vulkan Widgets)

target_include_directories (engine_vulkan PUBLIC ${Vulkan_INCLUDE_DIR})
target_link_libraries(engine_vulkan Qt5::Core Qt5::Gui)
target_compile_definitions(engine_vulkan PRIVATE USE_VULKAN)

#RULE FOR OPENGL
set(SRC_OPENGL
        ${SRC_DIR_OPENGL}/mainwindow.cpp
        ${SRC_DIR_OPENGL}/vbooglrenderer.cpp
        ${SRC_DIR_OPENGL}/oglwidget.cpp
        ${SRC_DIR_OPENGL}/openglrenderer.cpp
        ${SRC_DIR_OPENGL}/mainwindow.h
        )

add_executable(engine_opengl ${SRC_COMMON} ${SRC_OPENGL})
qt5_use_modules(engine_opengl Widgets)
target_link_libraries(engine_opengl -lGLEW)
target_link_libraries(engine_opengl Qt5::Core Qt5::Gui)
target_link_libraries(engine_opengl ${OPENGL_LIBRARIES} ${GLUT_LIBRARY} ${QT_LIBRARIES})

#Move shader for opengl
add_custom_command(TARGET engine_opengl POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy
        ${SRC_DIR_OPENGL}/shader.vert ${SRC_DIR_OPENGL}/shader.frag
        ${CMAKE_BINARY_DIR})

#Compile shader for Vulkan
find_program(GLSLC glslc)
add_custom_command(TARGET engine_vulkan POST_BUILD
        COMMAND ${GLSLC} ${SRC_DIR_VULKAN}/vulkan_shader.frag -o color_frag.spv
        COMMAND ${GLSLC} ${SRC_DIR_VULKAN}/vulkan_shader.vert -o color_vert.spv
        )
