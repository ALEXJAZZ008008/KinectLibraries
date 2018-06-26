PROJECT(KinectFrontend VERSION 0.0 LANGUAGES C CXX)

include_directories(${Freenect_INCLUDE_DIR})

add_executable(${PROJECT_NAME} "${CMAKE_CURRENT_LIST_DIR}/main.cpp")
target_link_libraries(${PROJECT_NAME} Qt5::Core Qt5::Widgets Qt5::Gui KinectBackend ${Freenect_LIBRARY})


 target_sources(KinectFrontend
    PRIVATE
        "${CMAKE_CURRENT_LIST_DIR}/KinectFrontend.cpp"
        "${CMAKE_CURRENT_LIST_DIR}/motor_control.ui"
        "${CMAKE_CURRENT_LIST_DIR}/KinectFrontend.h"
        )


install(TARGETS KinectFrontend DESTINATION bin)
