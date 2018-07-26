PROJECT(KinectInterface VERSION 0.0 LANGUAGES CXX)

add_library(KinectInterface ${CMAKE_CURRENT_LIST_DIR})

target_link_libraries(${PROJECT_NAME} KinectObject KinectBackend KinectInputOutput)

target_sources(KinectInterface 
	PRIVATE
	"${CMAKE_CURRENT_LIST_DIR}/KinectInterface.cpp") 

install(TARGETS KinectInterface DESTINATION lib)
