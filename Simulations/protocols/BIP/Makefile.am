ACLOCAL_AMFLAGS = -I m4

lib_LTLIBRARIES = librouting_bip.la

librouting_bip_la_CFLAGS = $(CFLAGS) $(GLIB_FLAGS) -Wall -I./include -I./src -I$(prefix)/include
librouting_bip_la_SOURCES = src/one_hop.c src/AddedFunction.c src/Initialisation.c src/Implementation.c src/bip.c

librouting_bip_la_LDFLAGS = -module -L$(prefix)/lib -lstructure

