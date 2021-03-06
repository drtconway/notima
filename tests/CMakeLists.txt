cmake_minimum_required(VERSION 3.1)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Explicitly list the test source code and headers. The Catch header-only unit
# test framework is stored in with the test source.
set(Notima_TEST_SRC
    test_bit_array.cpp
    test_fastq.cpp
    test_gzipf.cpp
    test_integer_array.cpp
    test_kmers.cpp
    test_poppy.cpp
    test_radix_array.cpp
    test_rrr_array.cpp
    test_sparse_array.cpp
    test_stats.cpp
    test_subword_array.cpp
    test_tsv.cpp
    test_vbit_array.cpp
    test_wordy.cpp
)
set(Notima_TEST_HEADER
    catch.hpp
)

PREPEND(Notima_TEST_SRC)

include_directories(nlohmann_json::nlohmann_json)

# Make an executable target that depends on the test source code we specified
# above.
add_executable(TestNotima ${Notima_TEST_SRC} ${Notima_TEST_HEADER})

# Enable testing via CTest
enable_testing()
# Add our test as runnable via CTest
add_test(NAME TestNotima  COMMAND TestNotima)

target_link_libraries(TestNotima ${ZLIB_LIBRARIES} nlohmann_json::nlohmann_json)
