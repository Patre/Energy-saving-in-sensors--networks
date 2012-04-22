ACLOCAL_AMFLAGS = -I m4

lib_LTLIBRARIES = librouting_flood.la

librouting_flood_la_CFLAGS = $(CFLAGS) $(GLIB_FLAGS) -Wall -I./include -I./src -I$(prefix)/include
librouting_flood_la_SOURCES = src/Implementation.c src/flood.c

librouting_flood_la_LDFLAGS = -module -L$(prefix)/lib -lstructure

