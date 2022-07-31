set(urlparse_objects
    charutils.cpp
    domainnamereader.cpp
    glog.cpp
    hostnormalizer.cpp
    inetaddress.cpp
    inputtextreader.cpp
    pathnormalizer.cpp
    stringbuilder.cpp
    stringutils.cpp
    url.cpp
    urldetector.cpp
    urldetectoroptions.cpp
    urlmarker.cpp
    urlpart.cpp
    urlutil.cpp
)

add_library(urlparse SHARED ${urlparse_objects})
target_link_libraries(urlparse pthread dl)

add_test(NAME test COMMAND echo "123")