dist_bin_SCRIPTS = wsnet-topogen

do_subst = sed -e 's,[@]datadir[@],$(datadir),g'

wsnet-topogen: topogen.in
	$(do_subst) < $(srcdir)/topogen.in > wsnet-topogen
	chmod +x wsnet-topogen

CLASSPATH_ENV = CLASSPATH=./:$${CLASSPATH}

JAVAROOT = .

javadir = $(datadir)/java

dist_java_JAVA = topogen.java
