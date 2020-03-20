#                                               -*- Autoconf -*-
# Process this file with autoconf to produce a configure script.

AC_PREREQ([2.69])
AC_INIT(liburlparse, 1.0, 13752913198@163.com)
AM_INIT_AUTOMAKE

# Checks for programs.
AC_PROG_CXX
#AC_PROG_CC
AC_PROG_RANLIB

AC_SUBST([DOLLAR_SIGN],[$])
# 在Makefile.am 使用Shell --> val=@DOLLAR_SIGN@(shell /bin/pwd) to avoid warning "non-POSIX variable name"

# Checks for libraries.
# FIXME: Replace `main' with a function in `-lgtest':
AC_CHECK_LIB(thread, unitmain)
AC_CHECK_LIB(gtest, unitmain)
# FIXME: Replace `main' with a function in `-lurlparse':

# Checks for header files.
AC_CHECK_HEADERS([stdlib.h string.h])

# Checks for typedefs, structures, and compiler characteristics.
AC_CHECK_HEADER_STDBOOL
AC_TYPE_OFF_T
AC_TYPE_SIZE_T

# Checks for library functions.
AC_CHECK_FUNCS([memset strtol])

AC_OUTPUT([Makefile 
           src/Makefile
           Test/Makefile
        ])
