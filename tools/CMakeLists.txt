add_library(pyparse SHARED export_url_parse.cpp)
target_include_directories(pyparse PRIVATE ${PROJECT_SOURCE_DIR}/include)
target_link_libraries(pyparse PRIVATE ld)
