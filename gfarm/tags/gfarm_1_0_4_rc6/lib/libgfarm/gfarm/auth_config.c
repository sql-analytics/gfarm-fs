#include <assert.h>
#include <string.h>
#include <stdlib.h>
#include <limits.h>
#include <gfarm/gfarm_config.h>
#include <gfarm/gfarm_error.h>
#include <gfarm/gfarm_misc.h>
#include "hostspec.h"
#include "auth.h"

/*
 * gfarm_auth_method
 */

struct gfarm_auth_method_name_value {
	char mnemonic;
	char *name;
	enum gfarm_auth_method method;
} gfarm_auth_method_name_value_table[] = {
	{ 's', "sharedsecret",	GFARM_AUTH_METHOD_SHAREDSECRET },
	{ 'G', "gsi",		GFARM_AUTH_METHOD_GSI },
	{ 'g', "gsi_auth",	GFARM_AUTH_METHOD_GSI_AUTH },
};

enum gfarm_auth_config_command { GFARM_AUTH_ENABLE, GFARM_AUTH_DISABLE };

struct gfarm_auth_config {
	struct gfarm_auth_config *next;

	enum gfarm_auth_config_command command;
	enum gfarm_auth_method method;
	struct gfarm_hostspec *hostspec;
};

struct gfarm_auth_config *gfarm_auth_config_list = NULL;
struct gfarm_auth_config **gfarm_auth_config_last = &gfarm_auth_config_list;

char
gfarm_auth_method_mnemonic(enum gfarm_auth_method method)
{
	int i;

	for (i = 0; i < GFARM_ARRAY_LENGTH(gfarm_auth_method_name_value_table);
	     i++) {
		struct gfarm_auth_method_name_value *entry =
		    &gfarm_auth_method_name_value_table[i];

		if (entry->method == method)
			return (entry->mnemonic);
	}
	return ('-');
}

char *
gfarm_auth_method_name(enum gfarm_auth_method method)
{
	int i;

	for (i = 0; i < GFARM_ARRAY_LENGTH(gfarm_auth_method_name_value_table);
	     i++) {
		struct gfarm_auth_method_name_value *entry =
		    &gfarm_auth_method_name_value_table[i];

		if (entry->method == method)
			return (entry->name);
	}
	return ("unknown auth method");
}

char *
gfarm_auth_method_parse(char *name, enum gfarm_auth_method *methodp)
{
	int i;

	for (i = 0; i < GFARM_ARRAY_LENGTH(gfarm_auth_method_name_value_table);
	     i++) {
		if (strcmp(name, gfarm_auth_method_name_value_table[i].name)
		    == 0) {
			*methodp =
			    gfarm_auth_method_name_value_table[i].method;
			return (NULL);
		}
	}
	return (GFARM_ERR_NO_SUCH_OBJECT);
}

char *
gfarm_auth_config_add(
	enum gfarm_auth_config_command command,
	enum gfarm_auth_method method,
	struct gfarm_hostspec *hsp)
{
	struct gfarm_auth_config *acp =
	    malloc(sizeof(struct gfarm_auth_config));

	if (acp == NULL)
		return (GFARM_ERR_NO_MEMORY);
	acp->next = NULL;
	acp->command = command;
	acp->method = method;
	acp->hostspec = hsp;

	*gfarm_auth_config_last = acp;
	gfarm_auth_config_last = &acp->next;
	return (NULL);
}

char *
gfarm_auth_enable(enum gfarm_auth_method method, struct gfarm_hostspec *hsp)
{
	return (gfarm_auth_config_add(GFARM_AUTH_ENABLE,
	    method, hsp));
}

char *
gfarm_auth_disable(enum gfarm_auth_method method, struct gfarm_hostspec *hsp)
{
	return (gfarm_auth_config_add(GFARM_AUTH_DISABLE,
	    method, hsp));
}

/* this i/f have to be changed, if we support more than 31 auth methods */
gfarm_int32_t
gfarm_auth_method_get_enabled_by_name_addr(
	char *name, struct sockaddr *addr)
{
	struct gfarm_auth_config *acp = gfarm_auth_config_list;
	gfarm_int32_t enabled = 0, disabled = 0, methods;

	assert(GFARM_AUTH_METHOD_NUMBER <= sizeof(gfarm_int32_t) * CHAR_BIT);

	for (; acp != NULL; acp = acp->next) {
		if (gfarm_hostspec_match(acp->hostspec, name, addr)) {
			if (acp->method == GFARM_AUTH_METHOD_ALL) {
				methods = ((1 << GFARM_AUTH_METHOD_NUMBER) - 1)
				    & ~(1 << GFARM_AUTH_METHOD_NONE)
				    & ~(enabled | disabled);
			} else {
				methods = 1 << acp->method;
				if (((enabled | disabled) & methods) != 0)
					continue; /* already determined */
			}
			switch (acp->command) {
			case GFARM_AUTH_ENABLE:
				enabled |= methods;
				break;
			case GFARM_AUTH_DISABLE:
				disabled |= methods;
				break;
			}
		}
	}
	return (enabled);
}

gfarm_int32_t
gfarm_auth_method_get_available(void)
{
	int i;
	gfarm_int32_t methods;

	assert(GFARM_AUTH_METHOD_NUMBER <= sizeof(gfarm_int32_t) * CHAR_BIT);

	methods = 0;
	for (i = GFARM_AUTH_METHOD_NONE + 1; i < GFARM_AUTH_METHOD_NUMBER;
	    i++) {
		switch (i) {
		case GFARM_AUTH_METHOD_GSI_OLD: /* obsolete */
			break;
#ifndef HAVE_GSI
		case GFARM_AUTH_METHOD_GSI:
			break;
		case GFARM_AUTH_METHOD_GSI_AUTH:
			break;
#endif
		default:
			methods |= 1 << i;
			break;
		}
	}
	return (methods);
}

/*
 * gfarm_auth_server_cred
 */

struct gfarm_auth_cred_type_name_value {
	char *name;
	enum gfarm_auth_cred_type type;
} gfarm_auth_cred_type_name_value_table[] = {
	{ "",			GFARM_AUTH_CRED_TYPE_DEFAULT },
	{ "no-name",		GFARM_AUTH_CRED_TYPE_NO_NAME },
	{ "mechanism-specific",	GFARM_AUTH_CRED_TYPE_MECHANISM_SPECIFIC },
	{ "host",		GFARM_AUTH_CRED_TYPE_HOST },
	{ "user",		GFARM_AUTH_CRED_TYPE_USER },
};

char *
gfarm_auth_cred_type_parse(char *type_name, enum gfarm_auth_cred_type *typep)
{
	int i;

	for (i = 0;
	     i < GFARM_ARRAY_LENGTH(gfarm_auth_cred_type_name_value_table);
	     i++) {
		struct gfarm_auth_cred_type_name_value *entry = 
		    &gfarm_auth_cred_type_name_value_table[i];

		if (strcmp(type_name, entry->name) == 0) {
			*typep = entry->type;
			return (NULL);
		}
	}
	return ("unknown cred_type");
}

struct gfarm_auth_cred_config {
	struct gfarm_auth_cred_config *next;
	char *service_tag;
	enum gfarm_auth_cred_type type;
	char *service;
	char *name;
};

struct gfarm_auth_cred_config *gfarm_auth_server_cred_config_list = NULL;

static struct gfarm_auth_cred_config **
gfarm_auth_server_cred_config_lookup(char *service_tag)
{
	struct gfarm_auth_cred_config *conf, **p;

	for (p = &gfarm_auth_server_cred_config_list;
	    (conf = *p) != NULL; p = &conf->next) {
		if (strcmp(service_tag, conf->service_tag) == 0)
			break;
	}
	return (p);
}

/* service_tag must be statically allocated */
static char *
gfarm_auth_server_cred_config_enter(char *service_tag,
	struct gfarm_auth_cred_config **confp)
{
	struct gfarm_auth_cred_config *conf,
	    **p = gfarm_auth_server_cred_config_lookup(service_tag);

	if (*p != NULL) {
		*confp = *p;
		return (NULL);
	}
	*p = conf = malloc(sizeof(*conf));
	if (conf == NULL)
		return (GFARM_ERR_NO_MEMORY);
	conf->next = NULL;
	conf->service_tag = service_tag;
	conf->type = GFARM_AUTH_CRED_TYPE_DEFAULT;
	conf->service = NULL;
	conf->name = NULL;
	*confp = conf;
	return (NULL);
}

enum gfarm_auth_cred_type
gfarm_auth_server_cred_type_get(char *service_tag)
{
	struct gfarm_auth_cred_config *conf = 
	    *gfarm_auth_server_cred_config_lookup(service_tag);

	if (conf == NULL)
		return (GFARM_AUTH_CRED_TYPE_DEFAULT);
	return (conf->type);
}

char *
gfarm_auth_server_cred_service_get(char *service_tag)
{
	struct gfarm_auth_cred_config *conf = 
	    *gfarm_auth_server_cred_config_lookup(service_tag);

	if (conf == NULL)
		return (NULL);
	return (conf->service);
}

char *
gfarm_auth_server_cred_name_get(char *service_tag)
{
	struct gfarm_auth_cred_config *conf = 
	    *gfarm_auth_server_cred_config_lookup(service_tag);

	if (conf == NULL)
		return (NULL);
	return (conf->name);
}

/* service_tag must be statically allocated */
char *
gfarm_auth_server_cred_type_set_by_string(char *service_tag, char *string)
{
	char *e;
	enum gfarm_auth_cred_type type;

	e = gfarm_auth_cred_type_parse(string, &type);
	if (e != NULL)
		return (e);
	return (gfarm_auth_server_cred_type_set(service_tag, type));
}

/* service_tag must be statically allocated */
char *
gfarm_auth_server_cred_type_set(char *service_tag,
	enum gfarm_auth_cred_type type)
{
	struct gfarm_auth_cred_config *conf;
	char *e = gfarm_auth_server_cred_config_enter(service_tag, &conf);

	if (e != NULL)
		return (e);
	/* first line has precedence */
	if (conf->type != GFARM_AUTH_CRED_TYPE_DEFAULT)
		return (NULL);
	conf->type = type;
	return (NULL);
}

/* service_tag must be statically allocated */
char *
gfarm_auth_server_cred_service_set(char *service_tag, char *service)
{
	struct gfarm_auth_cred_config *conf;
	char *e = gfarm_auth_server_cred_config_enter(service_tag, &conf);

	if (e != NULL)
		return (e);
	if (conf->service != NULL) /* first line has precedence */
		return (NULL);
	if ((conf->service = strdup(service)) == NULL)
		return (GFARM_ERR_NO_MEMORY);
	return (NULL);
}

/* service_tag must be statically allocated */
char *
gfarm_auth_server_cred_name_set(char *service_tag, char *name)
{
	struct gfarm_auth_cred_config *conf;
	char *e = gfarm_auth_server_cred_config_enter(service_tag, &conf);

	if (e != NULL)
		return (e);
	if (conf->name != NULL) /* first line has precedence */
		return (NULL);
	if ((conf->name = strdup(name)) == NULL)
		return (GFARM_ERR_NO_MEMORY);
	return (NULL);
}
