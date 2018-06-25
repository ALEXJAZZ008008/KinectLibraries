PROJECT(KinectOpenGL VERSION 0.0 LANGUAGES CXX)

add_library(KinectOpenGL ${CMAKE_CURRENT_LIST_DIR})

target_link_libraries(KinectOpenGL) 

target_sources(KinectOpenGL 
	PRIVATE
	"${CMAKE_CURRENT_LIST_DIR}/KinectOpenGL.cpp"
    # "${CMAKE_CURRENT_LIST_DIR}/KinectOpenGL.h"
    ) 

install(TARGETS KinectOpenGL DESTINATION lib)
