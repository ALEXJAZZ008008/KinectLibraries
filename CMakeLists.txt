 #   Copyright 2017
cmake_minimum_required(VERSION 3.1 FATAL_ERROR)

PROJECT(KinectLibraries VERSION 0.0 LANGUAGES C CXX)

list(APPEND CMAKE_MODULE_PATH ${CMAKE_SOURCE_DIR}/cmake)

find_package(Threads REQUIRED)

############################  Options: Build What?  ############################
option(DOWNLOAD_LibFreenect "Downlaod the Freenect libraries v.1" OFF)
option(DOWNLOAD_LibFreenect2 "Downlaod the Freenect libraries v.2" OFF)

# option(DOWNLOAD_Freenect "Get the Freenect libraries from git" OFF)
# option(ENABLE_QT "Enables the QT interface" OFF)

############################  Options: Build How?  #############################
include(commons)
option_with_print(BUILD_SHARED_LIBS "Build internally built libraries as shared, not static" OFF)
option_with_default(CMAKE_BUILD_TYPE "Build type (Release or Debug)" Debug)
option_with_default(CMAKE_INSTALL_LIBDIR "Directory to which libraries installed" lib)

if (CMAKE_COMPILER_IS_GNUCC)
	add_definitions(-Wall -Wno-deprecated)
	set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -Wall -std=gnu++0x -ffast-math")
endif ()

if((${CMAKE_CXX_COMPILER_ID} MATCHES "AppleClang") OR
   (APPLE AND ${CMAKE_SYSTEM_NAME} MATCHES "Darwin" AND
    ${CMAKE_CXX_COMPILER_ID} MATCHES "Clang"))
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -stdlib=libc++")
endif()

if(DOWNLOAD_LibFreenect)
	configure_file(${CMAKE_MODULE_PATH}/libfreenect.txt.in libfreenect-download/CMakeLists.txt)
	execute_process(COMMAND "${CMAKE_COMMAND}" -G "${CMAKE_GENERATOR}" . 
		WORKING_DIRECTORY "${CMAKE_BINARY_DIR}/libfreenect-download" )
	execute_process(COMMAND "${CMAKE_COMMAND}" --build . 
		WORKING_DIRECTORY "${CMAKE_BINARY_DIR}/libfreenect-download" )
	add_subdirectory("${CMAKE_BINARY_DIR}/libfreenect-src" 
		"${CMAKE_BINARY_DIR}/libfreenect-build")
	SET( FREENECT_FOUND "YES" )
        else()
                find_package(libfreenect REQUIRED)
                include_directories(${FREENECT_INCLUDE_DIR})
endif()

if(DOWNLOAD_LibFreenect2)

	configure_file(${CMAKE_MODULE_PATH}/libfreenect2.txt.in libfreenect2-download/CMakeLists.txt)
	execute_process(COMMAND "${CMAKE_COMMAND}" -G "${CMAKE_GENERATOR}" .
		WORKING_DIRECTORY "${CMAKE_BINARY_DIR}/libfreenect2-download" )
	execute_process(COMMAND "${CMAKE_COMMAND}" --build .
		WORKING_DIRECTORY "${CMAKE_BINARY_DIR}/libfreenect2-download" )
	add_subdirectory("${CMAKE_BINARY_DIR}/libfreenect2-src"
                "${CMAKE_BINARY_DIR}/libfreenect2-build")
#        else()
#                find_package(libfreenect REQUIRED)
#                include_directories(${FREENECT_INCLUDE_DIR})
endif()

add_library(KinectDepthImageOutputLibrary "")

include_directories(KinectDepthImageOutputLibrary)
include_directories(KinectMotorControlWrapper)

include(KinectDepthImageOutputLibrary/CMakeLists.txt)
include(KinectDepthImageOutputWrapper/CMakeLists.txt)
include(KinectMotorControlWrapper/CMakeLists.txt)


set(CONFIGURED_ONCE TRUE CACHE INTERNAL
     "A flag showing that CMake has configured at least once.")
