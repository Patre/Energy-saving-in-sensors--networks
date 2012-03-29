ACLOCAL_AMFLAGS=-I m4 

lib_LTLIBRARIES = librouting_lbip.la

librouting_lbip_la_CFLAGS = $(CFLAGS) $(GLIB_FLAGS) -Wall -I./include -I./src -I$(prefix)/include
librouting_lbip_la_SOURCES = src/one_hop.c src/two_hop.c src/BIP_tree_utils.c src/lbip.c

librouting_lbip_la_LDFLAGS = -module -L$(prefix)/lib -lstructure

