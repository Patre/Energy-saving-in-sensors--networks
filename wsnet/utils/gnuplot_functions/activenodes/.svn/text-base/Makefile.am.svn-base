dist_bin_SCRIPTS = activenodes

do_subst = sed -e 's,[@]datadir[@],$(datadir),g'

activenodes: activenodes.sh activenodes.in
	$(do_subst) < $(srcdir)/activenodes.sh > activenodes
	chmod +x activenodes

data_DATA = activenodes.in
