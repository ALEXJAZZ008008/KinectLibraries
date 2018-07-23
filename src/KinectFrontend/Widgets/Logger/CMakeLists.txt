PROJECT(Logger VERSION 0.0 LANGUAGES C CXX)

add_library(Logger ${CMAKE_CURRENT_LIST_DIR})

target_link_libraries(${PROJECT_NAME} Qt5::Core Qt5::Widgets Qt5::Gui KinectInterface)

target_sources(Logger
    PRIVATE
    "${CMAKE_CURRENT_LIST_DIR}/logger.cpp"
    "${CMAKE_CURRENT_LIST_DIR}/logger.ui"
    "${CMAKE_CURRENT_LIST_DIR}/logger.h")

install(TARGETS Logger DESTINATION lib)
