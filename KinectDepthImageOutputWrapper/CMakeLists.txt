PROJECT(KinectDepthImageOutputWrapper VERSION 0.0 LANGUAGES CXX)

# # Find the Qt components
find_package(Qt5 COMPONENTS Core Gui Widgets REQUIRED)

# # -- This is required in order to build QT elements
 set(CMAKE_AUTOMOC ON)
 set(CMAKE_AUTOUIC ON)
 set(CMAKE_AUTORCC ON)
 set(CMAKE_INCLUDE_CURRENT_DIR ON)

add_executable(${PROJECT_NAME} "${CMAKE_CURRENT_LIST_DIR}/main.cpp")

target_link_libraries(${PROJECT_NAME} Qt5::Core Qt5::Widgets Qt5::Gui KinectDepthImageOutputLibrary ${FREENECT_LIBRARY})

 target_sources(KinectDepthImageOutputWrapper
    PRIVATE
        "${CMAKE_CURRENT_LIST_DIR}/kinectdepthimageoutputwrapper.cpp"
        "${CMAKE_CURRENT_LIST_DIR}/kinectdepthimageoutputwrapper.h"
        )


install(TARGETS KinectDepthImageOutputWrapper DESTINATION bin)
