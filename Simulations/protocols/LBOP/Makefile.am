ACLOCAL_AMFLAGS = -I m4

lib_LTLIBRARIES = librouting_lbop.la

librouting_lbop_la_CFLAGS = $(CFLAGS) $(GLIB_FLAGS) -Wall -I./include -I./src -I$(prefix)/include
librouting_lbop_la_SOURCES = src/one_hop.c src/lmst.c src/AddedFunction.c src/Initialisation.c src/Implementation.c src/lbop.c

librouting_lbop_la_LDFLAGS = -module -L$(prefix)/lib -lstructure

