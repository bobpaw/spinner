#                                               -*- Autoconf -*-
# Process this file with autoconf to produce a configure script.

AC_PREREQ([2.69])
AC_INIT([luck], [0], [aiden.woodruff@gmail.com])
AC_CONFIG_SRCDIR([main.c])
AC_CONFIG_HEADERS([config.h])

# Checks for programs.
AC_PROG_CC
AC_PROG_CC_C99

# Checks for libraries.
# FIXME: Replace `main' with a function in `-lSDL2':
AC_CHECK_LIB([SDL2], [SDL_Init], [], [AC_MSG_ERROR([SDL2 is required])])
# FIXME: Replace `main' with a function in `-lSDL2_image':
AC_CHECK_LIB([SDL2_image], [IMG_Init], [], [AC_MSG_ERROR([SDL2-image is required])])
# FIXME: Replace `main' with a function in `-lSDL2_ttf':
AC_CHECK_LIB([SDL2_ttf], [TTF_Init], [], [AC_MSG_ERROR([SDL2-ttf is required])])
AC_CHECK_LIB([m], [pow])

# Checks for header files.
AC_CHECK_HEADERS([stdlib.h string.h])

# Checks for typedefs, structures, and compiler characteristics.
AC_CHECK_HEADER_STDBOOL

# Checks for library functions.
AC_CHECK_FUNCS([memset strtol])

AC_CONFIG_FILES([Makefile])
AM_INIT_AUTOMAKE
AC_OUTPUT
