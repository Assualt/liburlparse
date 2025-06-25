set(parse_objects
    char_utils.cpp
    domainname_reader.cpp
    log.cpp
    host_normalizer.cpp
    inet_address.cpp
    input_text_reader.cpp
    path_normalizer.cpp
    string_builder.cpp
    string_utils.cpp
    url.cpp
    url_detector.cpp
    url_detect_options.cpp
    url_marker.cpp
    url_part.cpp
    url_util.cpp
)

add_library(urlparse SHARED ${parse_objects})
target_include_directories(urlparse 
PUBLIC
    ${PROJECT_SOURCE_DIR}/include
)
target_link_libraries(urlparse pthread dl)

add_library(urlparse_static STATIC ${parse_objects})
target_include_directories(urlparse_static 
PUBLIC
    ${PROJECT_SOURCE_DIR}/include
)
target_link_libraries(urlparse_static pthread dl)

install(TARGETS urlparse urlparse_static DESTINATION lib)