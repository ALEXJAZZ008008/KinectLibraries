PROJECT(KinectInterface VERSION 0.0 LANGUAGES CXX)

add_library(KinectInterface ${CMAKE_CURRENT_LIST_DIR})

target_link_libraries(KinectInterface)

target_sources(KinectInterface 
	PRIVATE
	"${CMAKE_CURRENT_LIST_DIR}/KinectInterface.cpp"
    # "${CMAKE_CURRENT_LIST_DIR}/KinectInterface.h"
    ) 

install(TARGETS KinectInterface DESTINATION lib)
