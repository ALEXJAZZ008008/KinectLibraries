PROJECT(KinectBackend VERSION 0.0 LANGUAGES CXX)

add_library(KinectBackend ${CMAKE_CURRENT_LIST_DIR})

target_link_libraries(KinectBackend ${Freenect_LIBRARY} ${LIBUSB_1_LIBRARY})

target_sources(KinectBackend 
    PRIVATE
    "${CMAKE_CURRENT_LIST_DIR}/KinectBackend.cpp")

install(TARGETS KinectBackend DESTINATION lib)
