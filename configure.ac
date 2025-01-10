AC_INIT([dnf-plugin-diff], [2.0], [praiskup@redhat.com])
AM_INIT_AUTOMAKE([-Wall -Werror foreign])
AM_PROG_AR
AC_PROG_CXX([c++])
LT_INIT
AM_PATH_PYTHON
AC_ARG_ENABLE([dnf5],
              AS_HELP_STRING([--disable-dnf5], [Do not build against DNF5]))
AS_IF([test "x$enable_dnf5" != "xno"], [
PKG_CHECK_MODULES([LIBDNF5], [libdnf5-cli])
])
AM_CONDITIONAL([BUILD_WITH_DNF5], [test "x$enable_dnf5" != "xno"])
AC_CONFIG_FILES([Makefile dnf5-plugins/Makefile])
AC_OUTPUT
