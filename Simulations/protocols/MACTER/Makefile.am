ACLOCAL_AMFLAGS=-I m4 

lib_LTLIBRARIES = libmac_macter.la

libmac_macter_la_CFLAGS = $(CFLAGS) $(GLIB_FLAGS) -Wall -I./include -I./src -I$(prefix)/include
libmac_macter_la_SOURCES = src/macter.c
libmac_macter_la_LDFLAGS = -module -L$(prefix)/lib -lstructure
