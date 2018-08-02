PROJECT(KinectObject VERSION 0.0 LANGUAGES CXX)

add_library(KinectObject ${CMAKE_CURRENT_LIST_DIR})

target_link_libraries(${PROJECT_NAME})

target_sources(KinectObject
    PRIVATE
    "${CMAKE_CURRENT_LIST_DIR}/KinectObject.cpp")

install(TARGETS KinectObject DESTINATION lib)
