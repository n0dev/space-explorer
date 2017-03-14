cmake_minimum_required(VERSION 3.3)
project(space_explorer)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

include_directories(/usr/include/freetype2/)

#file(GLOB SOURCE_FILES
#        ${PROJECT_SOURCE_DIR}/sources/*.cpp)

set(SOURCE_FILES
    sources/data/importdb.cpp
    sources/data/importdb.h
    sources/data/objects.cpp
    sources/data/objects.h
    sources/data/planet.cpp
    sources/include/data/planet.h
    sources/data/spectrum.cpp
    sources/data/spectrum.h
    sources/gameplay/events.cpp
    sources/gameplay/events.h
    sources/gameplay/joystick.cpp
    sources/gameplay/keyboard.cpp
    sources/gameplay/mouse.cpp
    sources/gameplay/observer.cpp
    sources/gameplay/observer.h
    sources/gui/display.cpp
    sources/gui/window.cpp
    sources/include/gameplay.h
    sources/include/glsl.h
    sources/include/gui.h
    sources/include/music.h
    sources/include/quaternions.h
    sources/include/utils.h
    sources/sounds/music.cpp
    sources/textures/loadpng.cpp
    sources/textures/loadpng.h
    sources/textures/loadtiff.cpp
    sources/textures/loadtiff.h
    sources/utils/glsl.cpp
    sources/utils/quaternions.cpp
    sources/utils/utils.cpp
    sources/farbeyond.cpp)

add_executable(space_explorer ${SOURCE_FILES})

target_link_libraries(space_explorer SDL GL GLU GLEW openal png tiff sndfile ftgl)