


add_test(NAME test_buffer COMMAND tst_buffer)
add_test(NAME test_pthread COMMAND tst_pthread)

add_executable(tst_buffer "${CMAKE_CURRENT_LIST_DIR}/test_buffer.cxx")
#add_executable(tst_pthread "${CMAKE_CURRENT_LIST_DIR}/test_pthread.cxx")

#include(test_exe_targets)

