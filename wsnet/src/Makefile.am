SUBDIRS= scheduler

bin_PROGRAMS=wsnet

wsnet_CFLAGS= $(GLIB_FLAGS) $(XML_FLAGS) $(GSL_LIBS) $(GSL_FLAGS)
wsnet_SOURCES= main.c node.c config.c mobility.c modelutils.c  \
		topology.c rng.c probabilistic_distribution.c packet.c medium.c noise.c modulation.c bundle_config.c bundle.c entity.c entity_config.c measure.c measure_config.c environment_config.c node_config.c param.c monitor.c radio.c antenna.c battery.c ioctl_message.c
wsnet_LDADD= ../libraries/mem_fs/$(MEM_FS)/libmem_fs.a          \
	     ../libraries/das/$(DAS)/libdas.a                   \
	     ../libraries/sodas/$(SODAS)/libsodas.a             \
	     ../libraries/hadas/$(HADAS)/libhadas.a             \
	     ../libraries/spadas/$(SPADAS)/libspadas.a          \
	     ../libraries/timer/libtimer.a			\
	      ./scheduler/$(SCHEDULER)/libscheduler.a           \
	     ../libraries/worldsens/libwsens_srv.a              \
	      $(XML_LIBS) $(GLIB_LIBS) 

data_DATA = config.xsd
