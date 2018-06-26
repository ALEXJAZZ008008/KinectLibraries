add_test(NAME test_buffer COMMAND test_buffer)
add_test(NAME test_pthread COMMAND test_pthread)

add_executable(test_buffer "${CMAKE_CURRENT_LIST_DIR}/test_buffer.cxx")
add_executable(test_pthread "${CMAKE_CURRENT_LIST_DIR}/test_pthread.cxx")

