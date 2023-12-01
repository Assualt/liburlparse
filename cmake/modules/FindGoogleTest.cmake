include(ExternalProject)
message(STATUS "begin to find googletest")

set(GoogleTest_ROOT         ${PROJECT_SOURCE_DIR}/third_party/googletest)
# 指定配置指令（注意此处修改了安装目录，否则默认情况下回安装到系统目录）
set(GoogleTest_CONFIGURE    cd ${GoogleTest_ROOT}/ && mkdir install && cmake -D CMAKE_INSTALL_PREFIX=${GoogleTest_ROOT}/install .)
# 指定编译指令（需要覆盖默认指令，进入我们指定的GoogleTest_ROOT目录下）
set(GoogleTest_MAKE         cd ${GoogleTest_ROOT}/ && make) 
# 指定安装指令（需要覆盖默认指令，进入我们指定的GoogleTest_ROOT目录下）
set(GoogleTest_INSTALL      cd ${GoogleTest_ROOT}/ && make install)

ExternalProject_Add(GoogleTest
    PREFIX            ${GoogleTest_ROOT}
    SOURCE_DIR        ${GoogleTest_ROOT}
    CONFIGURE_COMMAND ${GoogleTest_CONFIGURE}
    BUILD_COMMAND     ${GoogleTest_MAKE}
    INSTALL_COMMAND   ${GoogleTest_INSTALL}
)

set(googletest_INCLUDE_DIR ${GoogleTest_ROOT}/install/include)
set(gtest ${GoogleTest_ROOT}/install/lib/libgtest.a)
set(gmock ${GoogleTest_ROOT}/install/lib/libgmock.a)
set(gtest_main ${GoogleTest_ROOT}/install/lib/libgtest_main.a)
set(gmock_main ${GoogleTest_ROOT}/install/lib/libgmock_main.a)
