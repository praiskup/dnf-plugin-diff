dnf5plugindir = $(libdir)/dnf5/plugins
dnf5plugin_LTLIBRARIES = diff.la
diff_la_SOURCES = diff.cc diff_cmd_plugin.cc
diff_la_LDFLAGS =  $(LIBDNF5_LDFLAGS) -module -avoid-version -shared
diff_la_CXXFLAGS = $(LIBDNF5_CXXFLAGS) -UPACKAGE -std=gnu++20 -Wl,--as-needed -fvisibility-inlines-hidden
