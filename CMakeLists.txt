cmake_minimum_required(VERSION 3.16)

project(QtQuickSDL VERSION 0.1 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

find_package(Qt6 COMPONENTS Core Quick REQUIRED)
find_package(SDL2 REQUIRED)
find_package(SDL2_image)

include_directories(${SDL2_INCLUDE_DIRS} ${SDL2_IMAGE_INCLUDE_DIRS})

set(TARGET_NAME Qt${QT_MAJOR_VERSION}QuickSDL)

add_subdirectory(src)
add_subdirectory(tests)
