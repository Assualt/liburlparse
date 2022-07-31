set(test_urlprase_objects
    testCharUtils.cpp
    testHostnormalizer.cpp
    testInputReader.cpp
    testNormalizedUrl.cpp
    testPathNormalizer.cpp
    testUrl.cpp
    unitmain.cpp
)
add_executable(unitmain ${test_urlprase_objects})
target_link_libraries(unitmain urlparse gtest)

add_test(NAME unitmain COMMAND unitmain)

add_executable(urlclient urlparseclient.cpp)
target_link_libraries(urlclient urlparse)

add_executable(testmain testmain.cpp)
target_link_libraries(testmain urlparse)