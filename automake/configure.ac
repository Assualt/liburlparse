#                                               -*- Autoconf -*-
# Process this file with autoconf to produce a configure script.

AC_PREREQ([2.69])
AC_INIT(liburlparse, 1.0, 13752913198@163.com)
AM_INIT_AUTOMAKE(liburlparse unitmain testmain, 1.0)

# Checks for programs.
AC_PROG_CXX
#AC_PROG_CC
AC_PROG_RANLIB

# Checks for libraries.
# FIXME: Replace `main' with a function in `-lgtest':
AC_CHECK_LIB(gtest, pthread )
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
