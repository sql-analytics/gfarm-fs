#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <gfarm/gfarm.h>

int
main(int argc, char **argv)
{
	gfarm_error_t e;
	GFS_File gf;
	int n;
	struct gfs_stat gst;

	e = gfarm_initialize(&argc, &argv);
	if (e != GFARM_ERR_NO_ERROR) {
		fprintf(stderr, "gfarm_initialize: %s\n",
		    gfarm_error_string(e));
		return (2);
	}

	if (argc != 4) {
		fprintf(stderr, "Usage: "
			"truncate <gfarm_url> <string> <length>\n");
		return (2);
	}
	e = gfs_pio_create(argv[1],
			   GFARM_FILE_RDWR|GFARM_FILE_TRUNC, 0666, &gf);
	if (e != GFARM_ERR_NO_ERROR) {
		fprintf(stderr, "gfs_pio_create: %s\n", gfarm_error_string(e));
		return (2);
	}
	e = gfs_pio_write(gf, argv[2], strlen(argv[2]), &n);
	if (e != GFARM_ERR_NO_ERROR) {
		fprintf(stderr, "gfs_pio_write: %s\n", gfarm_error_string(e));
		return (2);
	}
	e = gfs_pio_close(gf);
	if (e != GFARM_ERR_NO_ERROR) {
		fprintf(stderr, "gfs_pio_close: %s\n", gfarm_error_string(e));
		return (2);
	}

	e = gfs_pio_open(argv[1], GFARM_FILE_WRONLY, &gf);
	if (e != GFARM_ERR_NO_ERROR) {
		fprintf(stderr, "gfs_pio_open: %s\n", gfarm_error_string(e));
		return (2);
	}
	e = gfs_pio_truncate(gf, atol(argv[3]));
	if (e != GFARM_ERR_NO_ERROR) {
		fprintf(stderr, "%s\n", gfarm_error_string(e));
		return (1);
	}
	e = gfs_pio_close(gf);
	if (e != GFARM_ERR_NO_ERROR) {
		fprintf(stderr, "gfs_pio_close: %s\n", gfarm_error_string(e));
		return (2);
	}

	e = gfs_pio_open(argv[1], GFARM_FILE_RDONLY, &gf);
	if (e != GFARM_ERR_NO_ERROR) {
		fprintf(stderr, "gfs_pio_open: %s\n", gfarm_error_string(e));
		return (2);
	}
	e = gfs_stat(argv[1], &gst);
	if (e != GFARM_ERR_NO_ERROR) {
		fprintf(stderr, "%s\n", gfarm_error_string(e));
		return (1);
	}
	printf("%lld", gst.st_size);
	gfs_stat_free(&gst);
	e = gfs_pio_close(gf);
	if (e != GFARM_ERR_NO_ERROR) {
		fprintf(stderr, "gfs_pio_close: %s\n", gfarm_error_string(e));
		return (2);
	}

	e = gfarm_terminate();
	if (e != GFARM_ERR_NO_ERROR) {
		fprintf(stderr, "gfarm_terminate: %s\n",
		    gfarm_error_string(e));
		return (2);
	}
	return (0);
}
