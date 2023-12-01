find_package(GoogleTest REQUIRED)
include(CTest)
set(test_urlprase_objects
    test_char_utils.cpp
    test_host_normalizer.cpp
    test_input_reader.cpp
    test_normalized_url.cpp
    test_path_normalizer.cpp
    test_url.cpp
    unit_main.cpp
)

add_executable(unitmain ${test_urlprase_objects})
target_link_libraries(unitmain
PRIVATE
    urlparse 
    gtest
    gtest_main
)

add_test(NAME unitmain COMMAND unitmain)

add_executable(urlclient url_parse_client.cpp)
target_link_libraries(urlclient urlparse)

add_executable(example example_main.cpp)
target_link_libraries(example urlparse)
add_test(NAME example COMMAND example)