#include <stddef.h>
#include <unistd.h>

#define GFARM_INTERNAL_USE
#include <gfarm/gfarm.h>

#include "gfutil.h"

#include "gfm_client.h"
#include "config.h"
#include "lookup.h"

struct gfm_symlink_closure {
	/* input */
	const char *src;
	const char *path;	/* for gfarm_file_trace */
};

static gfarm_error_t
gfm_symlink_request(struct gfm_connection *gfm_server, void *closure,
	const char *base)
{
	struct gfm_symlink_closure *c = closure;
	gfarm_error_t e;

	if ((e = gfm_client_symlink_request(gfm_server, c->src, base))
	    != GFARM_ERR_NO_ERROR) {
		gflog_warning(GFARM_MSG_1000155,
		    "symlink(%s, %s) request: %s", c->src, base,
		    gfarm_error_string(e));
	}
	return (e);
}

static gfarm_error_t
gfm_symlink_result(struct gfm_connection *gfm_server, void *closure)
{
	struct gfm_symlink_closure *c = closure;
	int source_port;
	gfarm_error_t e;

	if ((e = gfm_client_symlink_result(gfm_server)) != GFARM_ERR_NO_ERROR) {
#if 0 /* DEBUG */
		gflog_debug(GFARM_MSG_1000156,
		    "symlink result: %s", gfarm_error_string(e));
#endif
	} else {
		if (gfarm_file_trace) {
			gfm_client_source_port(gfm_server, &source_port);
			gflog_trace(GFARM_MSG_1003272,
			    "%s/%s/%s/%d/SYMLINK/%s/%d/////\"%s\"///\"%s\"",
			    gfarm_get_local_username(),
			    gfm_client_username(gfm_server),
			    gfarm_host_get_self_name(), source_port,
			    gfm_client_hostname(gfm_server),
			    gfm_client_port(gfm_server),
			    c->src, c->path);
		}
	}
	return (e);
}

gfarm_error_t
gfs_symlink(const char *src, const char *path)
{
	struct gfm_symlink_closure closure;

	closure.src = src;
	closure.path = path;
	return (gfm_name_op(path, GFARM_ERR_OPERATION_NOT_PERMITTED,
	    gfm_symlink_request,
	    gfm_symlink_result,
	    gfm_name_success_op_connection_free,
	    &closure));
}
