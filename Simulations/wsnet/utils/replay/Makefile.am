dist_bin_SCRIPTS = wsnet-replay

do_subst = sed -e 's,[@]datadir[@],$(datadir),g'

wsnet-replay: replay.in
	$(do_subst) < $(srcdir)/replay.in > wsnet-replay
	chmod +x wsnet-replay

CLASSPATH_ENV = CLASSPATH=./:$${CLASSPATH}

JAVAROOT = .

javadir = $(datadir)/java

dist_java_JAVA = drawtopo.java frame.java replay.java

dist_java_DATA = exit.gif pause.gif play.gif
