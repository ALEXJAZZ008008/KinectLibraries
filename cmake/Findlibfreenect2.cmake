# - Try to find libfreenect2
# Once done, this will define
#
#  Freenect_2_FOUND - system has libfreenect2
#  Freenect_2_INCLUDE_DIRS - the libfreenect2 include directories
#  Freenect_2_LIBRARIES - link these to use libfreenect2

include(LibFindMacros)

# Use pkg-config to get hints about paths
# libfind_pkg_check_modules(Freenect_PKGCONF libfreenect2)

# IF(NOT FREENECT_2_ROOT)
#   IF(EXISTS "/usr/include/libfreenect2")
#     SET(FREENECT_2_ROOT "/usr")
#   ELSEIF(EXISTS "/usr/local/include/libfreenect2")
#     SET(FREENECT_2_ROOT "/usr/local")
#   ELSE()
#     MESSAGE("FREENECT_2_ROOT not set. Continuing anyway..")
#   ENDIF()
# ENDIF()

# Include dir
find_path(Freenect_2_INCLUDE_DIR
  NAMES libfreenect2.h
  PATHS ${FREENECT_2_ROOT}/include/libfreenect2 ${Freenect_2_PKGCONF_INCLUDE_DIRS}
)

# Finally the library itself
find_library(Freenect_2_LIBRARY
  NAMES freenect_2
  PATHS ${FREENECT_2_ROOT}/lib ${Freenect_2_PKGCONF_LIBRARY_DIRS}
)

find_library(FreenectSync_2_LIBRARY
  NAMES freenect_2_sync
  PATHS ${FREENECT_2_ROOT}/lib ${Freenect_2_PKGCONF_LIBRARY_DIRS}
)

# Set the include dir variables and the libraries and let libfind_process do the rest.
# NOTE: Singular variables for this library, plural for libraries this this lib depends on.
set(Freenect_2_PROCESS_INCLUDES Freenect_2_INCLUDE_DIR Freenect_2_INCLUDE_DIRS)
set(Freenect_2_PROCESS_LIBS FreenectSync_2_LIBRARY Freenect_2_LIBRARY Freenect_2_LIBRARIES)
libfind_process(Freenect2)
 
