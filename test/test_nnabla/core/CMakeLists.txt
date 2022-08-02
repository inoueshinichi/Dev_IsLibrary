cmake_minimum_required(VERSION 3.14.6)

include(${CMAKE_CURRENT_SOURCE_DIR}/../../cmake/test_case.cmake)


make_test_case(test_nnabla_core_context.cpp TEST_NNABLA_CORE_CONTEXT)
make_test_case(test_nnabla_core_memory.cpp TEST_NNABLA_CORE_MEMORY)
make_test_case(test_nnabla_core_allocator.cpp TEST_NNABLA_CORE_ALLOCATOR)
make_test_case(test_nnabla_core_caching_allocator.cpp TEST_NNABLA_CORE_CACHING_ALLOCATOR)
make_test_case(test_nnabla_core_array.cpp TEST_NNABLA_CORE_ARRAY)
make_test_case(test_nnabla_core_synced_array.cpp TEST_NNABLA_CORE_SYNCED_ARRAY)
make_test_case(test_nnabla_core_ndarray.cpp TEST_NNABLA_CORE_NDARRAY)







