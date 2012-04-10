ACLOCAL_AMFLAGS = -I m4

lib_LTLIBRARIES = librouting_rbop.la

librouting_rbop_la_CFLAGS = $(CFLAGS) $(GLIB_FLAGS) -Wall -I./include -I./src -I$(prefix)/include
librouting_rbop_la_SOURCES = src/one_hop.c src/Initialisation.c src/Implementation.c src/rbop.c

librouting_rbop_la_LDFLAGS = -module -L$(prefix)/lib -lstructure

