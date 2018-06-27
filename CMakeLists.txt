# Copyright 2018
# University of Hull

cmake_minimum_required(VERSION 3.1 FATAL_ERROR)

PROJECT(KinectLibraries VERSION 0.0 LANGUAGES CXX)

set (CMAKE_CXX_STANDARD 11)

list(APPEND CMAKE_MODULE_PATH ${CMAKE_SOURCE_DIR}/cmake)

############################  Options: Build What?  ############################
option(DOWNLOAD_LibFreenect "Downlaod the Freenect libraries v.1" OFF)
option(DOWNLOAD_LibFreenect2 "Downlaod the Freenect libraries v.2" OFF)
option(LOCAL_LIBFREENECT "Use Local Freect libraries" OFF)
option(BUILD_DOC "Build documentation" ON)
# option(ENABLE_QT "Enables the QT interface" OFF)

if(UNIX)
# Find the Qt components
find_package(Qt5 COMPONENTS Core Gui Widgets REQUIRED)
endif()

# Threads are used in the LibFreenect sync library and
# we might use them too, in order to acquire data without
# freezing the system.
find_package(Threads REQUIRED)

if(APPLE AND EXISTS /usr/local/opt/qt5)
    # Homebrew installs Qt5 (up to at least 5.9.1) in
    # /usr/local/qt5, ensure it can be found by CMake since
    # it is not in the default /usr/local prefix.
    list(APPEND CMAKE_PREFIX_PATH "/usr/local/opt/qt5")
endif()

if(WIN32 AND EXISTS ${QTDIR})
    list(APPEND CMAKE_PREFIX_PATH "${QTDIR}")
endif()

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

    if(WIN32)
        configure_file(${CMAKE_MODULE_PATH}/libusb.txt.in libusb-download/CMakeLists.txt)
        execute_process(COMMAND "${CMAKE_COMMAND}" -G "${CMAKE_GENERATOR}" .
            WORKING_DIRECTORY "${CMAKE_BINARY_DIR}/libusb-download" )
        execute_process(COMMAND "${CMAKE_COMMAND}" --build .
            WORKING_DIRECTORY "${CMAKE_BINARY_DIR}/libfreenect-download" )
        add_subdirectory("${CMAKE_BINARY_DIR}/libusb-src"
            "${CMAKE_BINARY_DIR}/libusb-build")

        INCLUDE_DIRECTORIES(${CMAKE_BINARY_DIR}/libusb-src/include)
        set(LIBUSB_1_INCLUDE_DIR "${CMAKE_BINARY_DIR}/libusb-src/include")

        set(LIBUSB_1_LIB_DIR "${CMAKE_BINARY_DIR}/lib")
        set(prefix "")
        set(suffix ".lib")

        set(LIBUSB_1_LIBRARY "${LIBUSB_1_LIB_DIR}/${prefix}usb${suffix}")
    endif()

    # We need the LibFreenect libraries, either locally installed
    # or we will download them from git.
    if (NOT LOCAL_LIBFREENECT)
        if(NOT DOWNLOAD_LibFreenect AND NOT DOWNLOAD_LibFreenect2)
            message(STATUS "${PROJECT_NAME} depends on libfreenect. You must either have it installed locally or download them.")
        endif()
    else()
        set(FREENECT_ROOT "" CACHE PATH "Root path of libfreenect.")
        find_package(libfreenect)
    endif()

    # Then check if we should download.
    if(DOWNLOAD_LibFreenect)
        set(LOCAL_LIBFREENECT "OFF")

        configure_file(${CMAKE_MODULE_PATH}/libfreenect.txt.in libfreenect-download/CMakeLists.txt)
        if (WIN32)
            execute_process(COMMAND "${CMAKE_COMMAND}" -G "${CMAKE_GENERATOR}" -DBUILD_EXAMPLES=0FF.
                WORKING_DIRECTORY "${CMAKE_BINARY_DIR}/libfreenect-download" )
        else()
            execute_process(COMMAND "${CMAKE_COMMAND}" -G "${CMAKE_GENERATOR}" .
                WORKING_DIRECTORY "${CMAKE_BINARY_DIR}/libfreenect-download" )
        endif()
        execute_process(COMMAND "${CMAKE_COMMAND}" --build .
            WORKING_DIRECTORY "${CMAKE_BINARY_DIR}/libfreenect-download" )
        add_subdirectory("${CMAKE_BINARY_DIR}/libfreenect-src"
            "${CMAKE_BINARY_DIR}/libfreenect-build")

        INCLUDE_DIRECTORIES(${CMAKE_BINARY_DIR}/libfreenect-src/include)

        set(Freenect_LIB_DIR "${CMAKE_BINARY_DIR}/lib")

        if(WIN32)
            set(prefix "")
            set(suffix ".lib")
        elseif(APPLE)
            set(prefix "lib")
            set(suffix ".a")
        else()
            set(prefix "lib")
            set(suffix ".a")
        endif()

        set(Freenect_LIBRARY "${Freenect_LIB_DIR}/${prefix}freenect${suffix}")

        set(Freenect_FOUND ON)
    endif()

    if(DOWNLOAD_LibFreenect2)
        message(FATAL_ERROR "libfreenect2 is not supported, yet!")
        # configure_file(${CMAKE_MODULE_PATH}/libfreenect2.txt.in libfreenect2-download/CMakeLists.txt)
        # execute_process(COMMAND "${CMAKE_COMMAND}" -G "${CMAKE_GENERATOR}" .
        # 	WORKING_DIRECTORY "${CMAKE_BINARY_DIR}/libfreenect2-download" )
        # execute_process(COMMAND "${CMAKE_COMMAND}" --build .
        # 	WORKING_DIRECTORY "${CMAKE_BINARY_DIR}/libfreenect2-download" )
        # add_subdirectory("${CMAKE_BINARY_DIR}/libfreenect2-src"
        #                "${CMAKE_BINARY_DIR}/libfreenect2-build")
    endif()


    # For building the documentation only in Release mode.
    if (CMAKE_BUILD_TYPE MATCHES "^[Rr]elease")
        if (BUILD_DOC)
            find_package(Doxygen)
            if (DOXYGEN_FOUND)
                # set input and output files
                set(DOXYGEN_IN ${CMAKE_CURRENT_SOURCE_DIR}/docs/Doxyfile.in)
                set(DOXYGEN_OUT ${CMAKE_CURRENT_BINARY_DIR}/Doxyfile)

                # request to configure the file
                configure_file(${DOXYGEN_IN} ${DOXYGEN_OUT} @ONLY)
                message("Doxygen build started")

                # note the option ALL which allows to build the docs together with the application
                add_custom_target( doc_doxygen ALL
                    COMMAND ${DOXYGEN_EXECUTABLE} ${DOXYGEN_OUT}
                    WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
                    COMMENT "Generating API documentation with Doxygen"
                    VERBATIM )
            else (DOXYGEN_FOUND)
                message("Doxygen need to be installed to generate the doxygen documentation")
            endif()
        endif()
    endif()

    set(CMAKE_AUTOMOC ON)
    set(CMAKE_AUTOUIC ON)
    set(CMAKE_AUTORCC ON)
    set(CMAKE_INCLUDE_CURRENT_DIR ON)

    # add_library(KinectBackend "")

    include_directories(src/include)
    # include_directories(src/KinectFrontend)

    if (Freenect_FOUND)
        include(src/CMakeLists.txt)
    endif()

    set(CONFIGURED_ONCE TRUE CACHE INTERNAL
        "A flag showing that CMake has configured at least once.")
