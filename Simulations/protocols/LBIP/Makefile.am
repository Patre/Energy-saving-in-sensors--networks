ACLOCAL_AMFLAGS=-I m4 

lib_LTLIBRARIES = librouting_lbip.la

librouting_lbip_la_CFLAGS = $(CFLAGS) $(GLIB_FLAGS) -Wall -I./include -I./src
librouting_lbip_la_SOURCES = src/structure/list_connected.c src/structure/list.c src/structure/arbre.c src/structure/list_de_list.c src/structure/list_paquet.c src/structure/conversion.c src/structure/time_wsnet.c src/structure/listNodes.c src/structure/list_de_listNodes.c src/structure/graphe.c \
	src/BIP/one_hop.c src/BIP/two_hop.c src/BIP/BIP_tree_utils.c src/BIP/lbip.c

librouting_lbip_la_LDFLAGS = -module

