
find_package(Python REQUIRED)
message(STATUS "Building python bindings ${PYTHON_VERSION}")

list(APPEND CMAKE_MODULE_PATH ${PROJECT_SOURCE_DIR}/third_party/pybind11)
message(STATUS "CMAKE_MODULE_PATH: ${CMAKE_MODULE_PATH}")

pybind11_add_module(pyurlparse
MODULE
    export_py.cpp
)

target_link_libraries(pyurlparse PRIVATE urlparse_static)
add_test(NAME pyurlparse_test COMMAND python3 ${CMAKE_CURRENT_SOURCE_DIR}/test_urlparse.py)
set_tests_properties(pyurlparse_test PROPERTIES ENVIRONMENT "PYTHONPATH=${CMAKE_ARCHIVE_OUTPUT_DIRECTORY}/../python")

install(TARGETS pyurlparse DESTINATION lib)