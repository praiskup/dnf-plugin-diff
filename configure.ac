AC_INIT([dnf-plugin-diff], [1.1], [praiskup@redhat.com])
AM_INIT_AUTOMAKE([-Wall -Werror foreign])
AM_PROG_AR
AC_PROG_CXX([c++])
LT_INIT
AM_PATH_PYTHON
PKG_CHECK_MODULES([LIBDNF5], [libdnf5-cli])
AC_CONFIG_FILES([Makefile dnf5-plugins/Makefile])
AC_OUTPUT
