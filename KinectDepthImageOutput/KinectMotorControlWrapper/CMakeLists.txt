PROJECT(KinectMotorControlWrapper VERSION 0.0 LANGUAGES CXX)

set(FREENECT_INCLUDE_DIR "default value" CACHE FILEPATH "description")
set(FREENECT_LIBRARY "default value" CACHE FILEPATH "description")

include_directories(${FREENECT_INCLUDE_DIR})

add_executable(${PROJECT_NAME} "${CMAKE_CURRENT_LIST_DIR}/main.cpp")

 target_sources(KinectMotorControlWrapper
    PRIVATE
        "${CMAKE_CURRENT_LIST_DIR}/KinectMotorControlWrapper.cpp"
        "${CMAKE_CURRENT_LIST_DIR}/KinectMotorControlWrapper.h"
        "${CMAKE_CURRENT_LIST_DIR}/motor_control.ui"
        )

target_link_libraries(${PROJECT_NAME} Qt5::Core Qt5::Widgets Qt5::Gui KinectDepthImageOutputLibrary ${FREENECT_LIBRARY})

install(TARGETS KinectMotorControlWrapper DESTINATION bin)
