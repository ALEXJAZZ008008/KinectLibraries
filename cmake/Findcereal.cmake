# - Try to find cereal
# Once done, this will define
#
#  Cereal_FOUND - system has cereal
#  Cereal_INCLUDE_DIRS - the cereal include directories
#  Cereal_LIBRARIES - link these to use cereal

include(LibFindMacros)

# Use pkg-config to get hints about paths
# libfind_pkg_check_modules(Cereal_PKGCONF cerea;)

# IF(NOT CEREAL_ROOT)
#   IF(EXISTS "/usr/include/cereal")
#     SET(CEREAL_ROOT "/usr")
#   ELSEIF(EXISTS "/usr/local/include/cereal")
#     SET(CEREAL_ROOT "/usr/local")
#   ELSE()
#     MESSAGE("CEREAL_ROOT not set. Continuing anyway..")
#   ENDIF()
# ENDIF()

# Include dir
find_path(Cereal_INCLUDE_DIR
  NAMES cereal.h
  PATHS ${CEREAL_ROOT}/include/cereal ${Cereal_PKGCONF_INCLUDE_DIRS}
)

# Finally the library itself
find_library(Cereal_LIBRARY
  NAMES cereal
  PATHS ${CEREAL_ROOT}/lib ${Ceral_PKGCONF_LIBRARY_DIRS}
)

find_library(CerealSync_LIBRARY
  NAMES cereal_sync
  PATHS ${CEREAL_ROOT}/lib ${Cereal_PKGCONF_LIBRARY_DIRS}
)

# Set the include dir variables and the libraries and let libfind_process do the rest.
# NOTE: Singular variables for this library, plural for libraries this this lib depends on.
set(Cereal_PROCESS_INCLUDES Cereal_INCLUDE_DIR Cereal_INCLUDE_DIRS)
set(Cereal_PROCESS_LIBS CerealSync_LIBRARY Cereal_LIBRARY Cereal_LIBRARIES)
libfind_process(Cereal)
 
