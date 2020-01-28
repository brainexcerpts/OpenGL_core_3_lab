project(minimal_renderer)

cmake_minimum_required(VERSION 3.10.2) # Minimal version compatible QT5
#CMAKE_POLICY(SET CMP0020 NEW) # This will silence the Cmake Warning "Policy CMP0020 is not set"

if(DEFINED CMAKE_COMPILER_IS_GNUCC)
    # Enable C++11
    SET( CMAKE_CXX_FLAGS  "${CMAKE_CXX_FLAGS} -std=c++0x" )
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wno-strict-aliasing")
    set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -O3")
endif()

set(EXECUTABLE_OUTPUT_PATH ${CMAKE_SOURCE_DIR}/bin)

find_package(OpenGL REQUIRED) # define OPENGL_LIBRARIES
find_package(Qt5Widgets REQUIRED)
# !! Widgets finds its own dependencies (QtGui and QtCore). !!
find_package(Qt5Core REQUIRED)
#find_package(Qt5Gui REQUIRED)
# #
find_package(Qt5OpenGL REQUIRED)

################################################################################
# Define project private sources and headers of rendersystem
#
# the variable "qtproject_SRCS" contains all .cpp files of this project
FILE(GLOB_RECURSE
    folder_source
    ${CMAKE_SOURCE_DIR}/src/rendersystem/renderer.cpp
    ${CMAKE_SOURCE_DIR}/src/gl_utils/*.cpp
    ${CMAKE_SOURCE_DIR}/src/gl_utils/glew/glew.c
    ${CMAKE_SOURCE_DIR}/src/fileloaders/*.cpp
    ${CMAKE_SOURCE_DIR}/src/qt_gui/*.cpp
    ${CMAKE_SOURCE_DIR}/src/main.cpp
    ${CMAKE_SOURCE_DIR}/src/timer.cpp
)

FILE(GLOB_RECURSE
    folder_header
    ${CMAKE_SOURCE_DIR}/src/*.h
    ${CMAKE_SOURCE_DIR}/src/*.hpp
    ${CMAKE_SOURCE_DIR}/src/rendersystem/*.h
    ${CMAKE_SOURCE_DIR}/src/fileloaders/*.h
    ${CMAKE_SOURCE_DIR}/src/gl_utils/*.h
    ${CMAKE_SOURCE_DIR}/src/gl_utils/glew/*.h
)

# QT object files need to be specified separately to generate the moc files
set(renderer_MOC_HDRS
    ${CMAKE_SOURCE_DIR}/src/qt_gui/mainwindow.h
    ${CMAKE_SOURCE_DIR}/src/qt_gui/openglwidget.h
)
QT5_WRAP_CPP(renderer_MOC_HDRS ${renderer_MOC_HDRS})

include_directories(
   ${CMAKE_SOURCE_DIR}/src
)


################################################################################
# Configure QT
include_directories(
   ${Qt5Widgets_INCLUDE_DIRS}
   ${Qt5OpenGL_INCLUDE_DIRS}
)

add_definitions(${Qt5Widgets_DEFINITIONS})
add_definitions(${Qt5OpenGL_DEFINITIONS})

set(QT_LIBS ${Qt5Widgets_LIBRARIES} ${Qt5OpenGL_LIBRARIES} ${Qt5Core_LIBRARIES} )

################################################################################
# Build target application

set(EXT_LIBS ${QT_LIBS} ${OPENGL_LIBRARIES})

add_executable(minimal_renderer
               ${folder_source}
               ${folder_header}
               ${renderer_MOC_HDRS}
               )

target_link_libraries(minimal_renderer ${EXT_LIBS} )

include(${CMAKE_CURRENT_SOURCE_DIR}/doxygen_setup.cmake)
