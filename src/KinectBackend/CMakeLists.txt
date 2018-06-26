PROJECT(KinectBackend VERSION 0.0 LANGUAGES CXX)

add_library(KinectBackend ${CMAKE_CURRENT_LIST_DIR})

target_link_libraries(${PROJECT_NAME} ${Freenect_LIBRARY})

target_sources(KinectBackend 
	PRIVATE
	"${CMAKE_CURRENT_LIST_DIR}/KinectBackend.cpp"
    # "${CMAKE_CURRENT_LIST_DIR}/KinectBackend.h"
    ) 

install(TARGETS KinectBackend DESTINATION lib)
