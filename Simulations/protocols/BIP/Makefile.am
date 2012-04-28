ACLOCAL_AMFLAGS=-I m4 

lib_LTLIBRARIES = librouting_bip.la

librouting_bip_la_CFLAGS = $(CFLAGS) $(GLIB_FLAGS) -Wall -I./include -I./src -I$(prefix)/include
librouting_bip_la_SOURCES = src/BIP_tree_utils.c src/bip.c

librouting_bip_la_LDFLAGS = -module -L$(prefix)/lib -lstructure

