SUBDIRS = data

plugin_LTLIBRARIES = oldsuv.la
oldsuv_la_LDFLAGS = -avoid-version -module -shared 
oldsuv_la_LIBADD = data/libimagedata.a
oldsuv_la_SOURCES = oldsuv.C 
AM_CXXFLAGS = $(LARGEFILE_CFLAGS)

INCLUDES = -I$(top_srcdir)/guicast -I$(top_srcdir)/cinelerra -I$(top_srcdir)/quicktime
LIBTOOL = $(SHELL) $(top_builddir)/libtool $(LTCXX_FLAGS)

noinst_HEADERS = oldsuv.h 
