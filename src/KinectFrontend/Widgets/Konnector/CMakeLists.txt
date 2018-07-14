PROJECT(Konnector VERSION 0.0 LANGUAGES C CXX)

include_directories(${Freenect_INCLUDE_DIR})

add_executable(${PROJECT_NAME} "${CMAKE_CURRENT_LIST_DIR}/main.cpp")

target_link_libraries(${PROJECT_NAME} Qt5::Core Qt5::Widgets Qt5::Gui KinectInterface Logger)

 target_sources(Konnector
    PRIVATE
        "${CMAKE_CURRENT_LIST_DIR}/konnector.cpp"
        "${CMAKE_CURRENT_LIST_DIR}/konnector.ui"
        "${CMAKE_CURRENT_LIST_DIR}/konnector.h"
        "${CMAKE_CURRENT_LIST_DIR}/konnector_settings.cpp"
        "${CMAKE_CURRENT_LIST_DIR}/konnector_settings.ui"
        "${CMAKE_CURRENT_LIST_DIR}/konnector_settings.h"
        )


#install(TARGETS Konnector DESTINATION bin)
