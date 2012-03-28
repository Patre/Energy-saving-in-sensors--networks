ACLOCAL_AMFLAGS=-I m4 

lib_LTLIBRARIES = librouting_bip.la  libapplication_app.la

librouting_bip_la_CFLAGS = $(CFLAGS) $(GLIB_FLAGS) -Wall -I./include -I./src
librouting_bip_la_SOURCES = src/structure/list_connected.c src/structure/list.c \
				src/structure/arbre.c src/structure/list_de_list.c \
				src/structure/list_paquet.c src/structure/conversion.c \
				src/structure/time_wsnet.c src/structure/listNodes.c \
                                src/structure/list_de_listNodes.c \
                                src/structure/one_hop.c \
                                src/BIP/AddedFunction.c src/BIP/Initialisation.c \
				src/BIP/Implementation.c src/BIP/bip.c

librouting_bip_la_LDFLAGS = -module


libapplication_app_la_CFLAGS = $(CFLAGS) $(GLIB_FLAGS) -Wall -I./include -I./src
libapplication_app_la_SOURCES = src/APP/app.c src/structure/time_wsnet.c
libapplication_app_la_LDFLAGS = -module
