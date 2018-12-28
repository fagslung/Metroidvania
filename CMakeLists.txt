cmake_minimum_required(VERSION 3.13)
project(Metroidvania)

set(CMAKE_CXX_STANDARD 17)

IF (WIN32)
    set(SDL2_PATH "C:\\home\\dev\\libs\\SDL2-2.0.9")
    set(SDL2_IMAGE_PATH "C:\\home\\dev\\libs\\SDL2_image-2.0.4")
    set(SDL2_MIXER_PATH "C:\\home\\dev\\libs\\SDL2_mixer-2.0.4")
    set(SDL2_TTF_PATH "C:\\home\\dev\\libs\\SDL2_ttf-2.0.14")
ENDIF()

# includes cmake/FindSDL2.cmake, etc.
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ${PROJECT_SOURCE_DIR}/cmake)

find_package(SDL2 REQUIRED)
find_package(SDL2_Image REQUIRED)
find_package(SDL2_ttf REQUIRED)
find_package(SDL2_Mixer REQUIRED)

include_directories(${SDL2_INCLUDE_DIR}
        ${SDL2_IMAGE_INCLUDE_DIR}
        ${SDL2_TTF_INCLUDE_DIR}
        ${SDL2_MIXER_INCLUDE_DIR})


set(SOURCE_FILES
        src/main.cpp
        src/intersect.cpp src/intersect.h
        src/dynamics.cpp src/dynamics.h src/gameutil.cpp src/gameutil.h)

add_executable(${CMAKE_PROJECT_NAME} ${SOURCE_FILES})

target_link_libraries(${CMAKE_PROJECT_NAME}
        ${SDL2_LIBRARY}
        ${SDL2_IMAGE_LIBRARIES}
        ${SDL2_TTF_LIBRARIES}
        ${SDL2_MIXER_LIBRARIES})

include_directories(src)

add_subdirectory(src)
add_subdirectory(test)