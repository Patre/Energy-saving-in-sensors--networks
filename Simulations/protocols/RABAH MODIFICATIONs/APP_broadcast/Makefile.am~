ACLOCAL_AMFLAGS=-I m4 

lib_LTLIBRARIES = libapplication_broadcast.la

libapplication_broadcast_la_CFLAGS = $(CFLAGS) $(GLIB_FLAGS) -Wall -I./include -I./src -I$(prefix)/include
libapplication_broadcast_la_SOURCES = src/app.c
libapplication_broadcast_la_LDFLAGS = -module -L$(prefix)/lib -lstructure
