/**
 *  \file   dbg.h
 *  \brief  dbg declarations
 *  \author Stephane D'Alu
 *  \date   2008
 **/
#ifndef __dbg__
#define __dbg__


#ifndef DEBUG_MAX
#  define DEBUG_MAX 9
#endif

#ifndef NODEBUG
#  define DBG(lvl, msg, ...) 						\
    if (lvl <= DBG_VAR) {						\
	fprintf(stderr, "%s: ", DBG_NAME);				\
	fprintf(stderr, msg "\n", ##  __VA_ARGS__);			\
    }
#  define NDBG(lvl, msg, c, ...)					\
    if (lvl <= DBG_VAR) {						\
	fprintf(stderr, "%s[%3d]: ", DBG_NAME, (c)->node);		\
	fprintf(stderr, msg "\n", ##  __VA_ARGS__);			\
    }
#else
#  define DBG(lvl, msg, ...)
#  define NDBG(lvl, msg, c, ...)
#endif

#if DEBUG_MAX >= 0
#  define DBG_CRIT(msg, ...)       DBG(0, msg, ## __VA_ARGS__)
#  define NDBG_CRIT(msg, c, ...)   NDBG(0, msg, c, ## __VA_ARGS__)
#else
#  define DBG_CRIT(msg, ...)
#  define NDBG_CRIT(msg, c, ...)
#endif

#if DEBUG_MAX >= 1
#  define DBG_WARN(msg, ...)       DBG(1, msg, ## __VA_ARGS__)
#  define NDBG_WARN(msg, ...)      NDBG(1, msg, c, ## __VA_ARGS__)
#else
#  define DBG_WARN(msg, ...)
#  define NDBG_WARN(msg, c, ...)
#endif

#if DEBUG_MAX >= 2
#  define DBG_INFO(msg, ...)       DBG(2, msg, ## __VA_ARGS__)
#  define NDBG_INFO(msg, c, ...)   NDBG(2, msg, c, ## __VA_ARGS__)
#else
#  define DBG_INFO(msg, ...)
#  define NDBG_INFO(msg, c, ...)
#endif

#if DEBUG_MAX >= 3
#  define DBG_VERB(msg, ...)       DBG(3, msg, ## __VA_ARGS__)
#  define NDBG_VERB(msg, c, ...)   NDBG(3, msg, c, ## __VA_ARGS__)
#else
#  define DBG_VERB(msg, ...)
#  define NDBG_VERB(msg, c, ...)
#endif

#if DEBUG_MAX >= 4
#  define DBG_NOISE(msg, ...)      DBG(4, msg, ## __VA_ARGS__)
#  define NDBG_NOISE(msg, c, ...)  NDBG(4, msg, c, ## __VA_ARGS__)
#else
#  define DBG_NOISE(msg, ...)
#  define NDBG_NOISE(msg, c, ...)
#endif

#if DEBUG_MAX >= 5
#  define DBG_XTRM(msg, ...)       DBG(5, msg, ## __VA_ARGS__)
#  define NDBG_XTRM(msg, c, ...)   NDBG(5, msg, c, ## __VA_ARGS__)
#else
#  define DBG_XTRM(msg, ...)
#  define NDBG_XTRM(msg, c, ...)
#endif


static inline 
void get_debug(call_t *c, char **entity_name, int *debug, int *log) {
    char *env  = getenv("WSNET_DEBUG");
    char *name = get_entity_name(c); 

    if (entity_name)
	*entity_name = name;

    if (env) {
	int   namelen = strlen(name);
	char *envstr  = strdup(env);
	char *oenvstr = envstr;
	char *token;
	while((token = strsep(&envstr, ","))) {
	    if ((! strncmp(name, token, namelen)) &&
		(token[namelen] == '=')) {
		char *value     = token+namelen+1;
		char *debug_str = strpbrk(value, "0123456789");
		char *log_str   = strpbrk(value, "lL");
		if (debug && debug_str)
		    *debug = debug_str[0] - '0';
		if (log   && log_str)
		    *log   = 1;
		break;
	    }
	}
	free(oenvstr);
    }
}

#endif
