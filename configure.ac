AC_INIT([rclauncher], [1.0], [rodrigorivascosta@gmail.com])
AC_CONFIG_AUX_DIR([auxdir])
AM_INIT_AUTOMAKE([-Wall -Werror foreign])

AC_SUBST(AM_CPPFLAGS, " ")
AC_SUBST(AM_CFLAGS, "-Wall")
AC_SUBST(AM_CXXFLAGS, "-Wall")


AC_PROG_CC
AC_PROG_CXX
AC_ARG_ENABLE([gtk3],
 [  --enable-gtk3          Use gtk+3 instead of gtk+2],
 [case "${enableval}" in
   yes) gtk3=true; ;;
   no) gtk3=false; ;;
   *) AC_MSG_ERROR([bad value ${enableval} for --enable-gtk3]) ;;
 esac],
 [gtk3=m4_ifdef([AM_PATH_GTK_3_0], [true], [false])]
)

AM_CONDITIONAL([GTK3], [test x$gtk3 == xtrue])
if test x$gtk3 == xtrue; then
    m4_ifdef([AM_PATH_GTK_3_0], [AM_PATH_GTK_3_0], [:])
else
    m4_ifdef([AM_PATH_GTK_2_0], [AM_PATH_GTK_2_0], [:])
fi

AC_CONFIG_HEADERS([config.h])
AC_CONFIG_FILES([Makefile src/Makefile])
AC_OUTPUT
