ACLOCAL_AMFLAGS=-I m4 

lib_LTLIBRARIES = libstructure.la

instdir = $(prefix)/lib

libstructure_la_CFLAGS = $(CFLAGS) $(GLIB_FLAGS) -Wall -I./include -I./src
libstructure_la_SOURCES = src/list_connected.c src/list.c src/arbre.c src/list_de_list.c src/list_paquet.c src/conversion.c src/time_wsnet.c src/listNodes.c src/list_de_listNodes.c src/graphe.c

libstructure_la_LDFLAGS = 

libstructure_la_includedir=$(prefix)/include
libstructure_la_include_HEADERS = include/arbre.h include/conversion.h include/graphe.h include/include.h include/list_connected.h include/list_de_list.h include/list_de_listNodes.h include/list_paquet.h include/list.h include/listNodes.h include/time_wsnet.h