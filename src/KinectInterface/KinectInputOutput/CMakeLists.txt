PROJECT(KinectInputOutput VERSION 0.0 LANGUAGES CXX)

add_library(KinectInputOutput ${CMAKE_CURRENT_LIST_DIR})

target_link_libraries(KinectInputOutput KinectObject)

target_sources(KinectInputOutput 
	PRIVATE
	"${CMAKE_CURRENT_LIST_DIR}/KinectInputOutput.cpp"
    # "${CMAKE_CURRENT_LIST_DIR}/KinectInputOutput.h"
    ) 

install(TARGETS KinectInputOutput DESTINATION lib)
