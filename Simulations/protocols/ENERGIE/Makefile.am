ACLOCAL_AMFLAGS = -I m4

lib_LTLIBRARIES = libenergy_theorie.la

libenergy_theorie_la_CFLAGS = $(GLIB_FLAGS) $(CFLAGS) $(GSL_FLAGS) -I$(top_srcdir) -I./src -I$(prefix)/include

libenergy_theorie_la_SOURCES = src/theorie.c

libenergy_theorie_la_LDFLAGS = -module -L$(prefix)/lib -lstructure


