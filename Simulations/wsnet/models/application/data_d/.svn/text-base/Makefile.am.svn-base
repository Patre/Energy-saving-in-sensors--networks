SUBDIRS = gossip ght xy lbdd

lib_LTLIBRARIES = libapplication_data_d_source.la libapplication_data_d_sink.la

libapplication_data_d_source_la_CFLAGS = $(GLIB_FLAGS) $(CFLAGS) $(GSL_FLAGS) -I$(top_srcdir)

libapplication_data_d_source_la_SOURCES = data_d_source.c

libapplication_data_d_source_la_LDFLAGS = -module

libapplication_data_d_sink_la_CFLAGS = $(GLIB_FLAGS) $(CFLAGS) $(GSL_FLAGS) -I$(top_srcdir)

libapplication_data_d_sink_la_SOURCES = data_d_sink.c

libapplication_data_d_sink_la_LDFLAGS = -module

