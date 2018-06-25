PROJECT(KinectDepthImageOutputWrapper VERSION 0.0 LANGUAGES CXX)


add_executable(${PROJECT_NAME} "${CMAKE_CURRENT_LIST_DIR}/main.cpp")

target_link_libraries(${PROJECT_NAME} Qt5::Core KinectDepthImageOutputLibrary ${FREENECT_LIBRARY})

 target_sources(KinectDepthImageOutputWrapper
    PRIVATE
        "${CMAKE_CURRENT_LIST_DIR}/kinectdepthimageoutputwrapper.cpp"
        # "${CMAKE_CURRENT_LIST_DIR}/kinectdepthimageoutputwrapper.h"
        )


install(TARGETS KinectDepthImageOutputWrapper DESTINATION bin)
