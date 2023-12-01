if (NOT BUILD_BENCHMARK)
    message(STATUS "not support benchmark")
endif()

add_executable(bench_strbuilder main.cpp)
target_link_libraries(bench_strbuilder PRIVATE benchmark)
target_include_directories(bench_strbuilder PRIVATE ${benchmark_SOURCE_DIR}/include)
add_test(NAME bench_strbuilder COMMAND bench_strbuilder)