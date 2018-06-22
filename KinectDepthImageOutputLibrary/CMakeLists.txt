PROJECT(KinectDepthImageOutputLibrary VERSION 0.0 LANGUAGES CXX)

target_link_libraries(KinectDepthImageOutputLibrary) 

target_sources(KinectDepthImageOutputLibrary 
	PRIVATE
	"${CMAKE_CURRENT_LIST_DIR}/kinectdepthimageoutputlibrary.cpp"
    "${CMAKE_CURRENT_LIST_DIR}/kinectdepthimageoutputlibrary.h"
    ) 


install(TARGETS KinectDepthImageOutputLibrary DESTINATION lib)
