#include <stddef.h>
#include <stdlib.h>
#include <string.h>
#include <gfarm/gfarm_config.h>
#include <gfarm/gfarm_error.h>
#include <gfarm/gfarm_misc.h>
#include <gfarm/gfarm_stringlist.h>

#define GFARM_STRINGLIST_INITIAL	50
#define GFARM_STRINGLIST_DELTA		50

/* gfarm_stringlist: variable size string array */

char *
gfarm_stringlist_init(gfarm_stringlist *listp)
{
	char **v;

	v = malloc(sizeof(char *) * GFARM_STRINGLIST_INITIAL);
	if (v == NULL)
		return (GFARM_ERR_NO_MEMORY);
	listp->size = GFARM_STRINGLIST_INITIAL;
	listp->length = 0;
	listp->array = v;
	v[0] = NULL;
	return (NULL);
}

void
gfarm_stringlist_free(gfarm_stringlist *listp)
{
	free(listp->array);

	/* the following is not needed, but to make erroneous program abort */
	listp->size = 0;
	listp->length = 0;
	listp->array = NULL;
}

void
gfarm_stringlist_free_deeply(gfarm_stringlist *listp)
{
	int i, length = gfarm_stringlist_length(listp);

	for (i = 0; i < length; i++) {
		if (listp->array[i] != NULL)
			free(listp->array[i]);
	}
	gfarm_stringlist_free(listp);
}

char *
gfarm_stringlist_add_strings(gfarm_stringlist *listp, int al, char **av)
{
	int ll = gfarm_stringlist_length(listp);

	if (ll + al > listp->size) {
		int n = listp->size;
		char **t;

		do {
			n += GFARM_STRINGLIST_DELTA;
		} while (ll + al > n);
		t = realloc(listp->array, sizeof(char *) * n);
		if (t == NULL)
			return (GFARM_ERR_NO_MEMORY);
		listp->size = n;
		listp->array = t;
	}
	memcpy(&listp->array[ll], av, sizeof(char *) * al);
	listp->length += al;
	return (NULL);
}

char *
gfarm_stringlist_add_list(gfarm_stringlist *listp, gfarm_stringlist *addp)
{
	return (gfarm_stringlist_add_strings(listp,
	    gfarm_stringlist_length(addp), addp->array));
}

char *
gfarm_stringlist_add(gfarm_stringlist *listp, char *s)
{
	int length = gfarm_stringlist_length(listp);

	if (length >= listp->size) {
		int n = listp->size + GFARM_STRINGLIST_DELTA;
		char **t = realloc(listp->array, sizeof(char *) * n);

		if (t == NULL)
			return (GFARM_ERR_NO_MEMORY);
		listp->size = n;
		listp->array = t;
	}
	listp->array[length] = s;
	listp->length++;
	return (NULL);
}

char *
gfarm_stringlist_cat(gfarm_stringlist *listp, char **v)
{
	return (gfarm_stringlist_add_strings(listp,
	    gfarm_strarray_length(v), v));
}

/* gfarm_fixedstrings: fixed length array of dynamically allocated strings */

char *
gfarm_fixedstrings_dup(int n, char **dst, char **src)
{
	int i;

	for (i = 0; i < n; i++) {
		dst[i] = strdup(src[i]);
		if (dst[i] == NULL) {
			while (--i >= 0) {
				free(dst[i]);
				dst[i] = NULL;
			}
			return (GFARM_ERR_NO_MEMORY);
		}
	}
	return (NULL);
}


/* gfarm_strings: dynamically allocated string array and array contents */

char **
gfarm_strings_alloc_from_stringlist(gfarm_stringlist *listp)
{
	int n = gfarm_stringlist_length(listp);
	char **t = malloc(sizeof(char *) * n);

	if (t == NULL)
		return (NULL);
	memcpy(t, listp->array, sizeof(char *) * n);
	return (t);
}

void
gfarm_strings_free_deeply(int n, char **strings)
{
	int i;

	for (i = 0; i < n; i++) {
		if (strings[i] != NULL)
			free(strings[i]);
	}
	free(strings);
}

/* gfarm_array: NULL terminated gfarm_strings */

int
gfarm_strarray_length(char **array)
{
	int i;

	for (i = 0; array[i] != NULL; i++)
		;
	return (i);
}

char **
gfarm_strarray_dup(char **array)
{
	int n = gfarm_strarray_length(array);
	char **v = malloc(sizeof(char *) * (n + 1));

	if (v == NULL)
		return (v);
	if (gfarm_fixedstrings_dup(n, v, array) != NULL)
		return (NULL);
	v[n] = NULL;
	return (v);
}

void
gfarm_strarray_free(char **array)
{
	int i;

	for (i = 0; array[i] != NULL; i++)
		free(array[i]);
	free(array);
}
