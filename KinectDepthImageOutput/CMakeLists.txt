 #   Copyright 2017

cmake_minimum_required(VERSION 3.1 FATAL_ERROR)

PROJECT(KinectLibraries VERSION 0.0 LANGUAGES CXX)

if (CMAKE_COMPILER_IS_GNUCC)
	add_definitions(-Wall -Wno-deprecated)
	set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -Wall -std=gnu++0x -ffast-math")
endif ()

if((${CMAKE_CXX_COMPILER_ID} MATCHES "AppleClang") OR
   (APPLE AND ${CMAKE_SYSTEM_NAME} MATCHES "Darwin" AND
    ${CMAKE_CXX_COMPILER_ID} MATCHES "Clang"))
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -stdlib=libc++")
endif()

# Find the Qt components
find_package(Qt5 COMPONENTS Core Gui Widgets REQUIRED)

set(FREENECT_INCLUDE_DIR "default value" CACHE FILEPATH "description")
set(FREENECT_LIBRARY "default value" CACHE FILEPATH "description")

include_directories(${FREENECT_INCLUDE_DIR})

# -- This is required in order to build QT elements
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_INCLUDE_CURRENT_DIR ON)

add_library(KinectDepthImageOutputLibrary "")

include_directories(KinectDepthImageOutputLibrary)
include_directories(KinectMotorControlWrapper)


include(KinectDepthImageOutputLibrary/CMakeLists.txt)
include(KinectDepthImageOutputWrapper/CMakeLists.txt)
include(KinectMotorControlWrapper/CMakeLists.txt)


set(CONFIGURED_ONCE TRUE CACHE INTERNAL
    "A flag showing that CMake has configured at least once.")