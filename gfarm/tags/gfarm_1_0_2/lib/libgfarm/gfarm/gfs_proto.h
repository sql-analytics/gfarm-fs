#ifndef GFSD_DEFAULT_PORT
#define GFSD_DEFAULT_PORT	600
#endif
#ifndef XAUTH_COMMAND
#define XAUTH_COMMAND "xauth"
#endif

enum gfs_proto_command {
	GFS_PROTO_OPEN,
	GFS_PROTO_CLOSE,
	GFS_PROTO_SEEK,
	GFS_PROTO_READ,
	GFS_PROTO_WRITE,
	GFS_PROTO_UNLINK,
	GFS_PROTO_CHDIR,
	GFS_PROTO_MKDIR,
	GFS_PROTO_RMDIR,
	GFS_PROTO_CHMOD,
	GFS_PROTO_CHGRP,
	GFS_PROTO_STAT,
	GFS_PROTO_DIGEST,
	GFS_PROTO_GET_SPOOL_ROOT,
	GFS_PROTO_BULKREAD,
	GFS_PROTO_BULKWRITE,
	GFS_PROTO_REPLICATE_FILE_SEQUENTIAL,
	GFS_PROTO_COMMAND,
	GFS_PROTO_REPLICATE_FILE_PARALLEL,
	GFS_PROTO_STRIPING_READ,
	GFS_PROTO_EXIST
};

enum gfs_proto_error {
	GFS_ERROR_NOERROR,
	GFS_ERROR_PERM,
	GFS_ERROR_NOENT,
	GFS_ERROR_SRCH,
	GFS_ERROR_INTR,
	GFS_ERROR_IO,
	GFS_ERROR_NXIO,
	GFS_ERROR_E2BIG,
	GFS_ERROR_NOEXEC,
	GFS_ERROR_BADF,
	/* ECHILD, */
	GFS_ERROR_DEADLK,
	GFS_ERROR_NOMEM,
	GFS_ERROR_ACCES,
	GFS_ERROR_FAULT,
	/* ENOTBLK, */
	GFS_ERROR_BUSY,
	GFS_ERROR_EXIST,
	/* EXDEV, */
	/* ENODEV, */
	GFS_ERROR_NOTDIR,
	GFS_ERROR_ISDIR,
	GFS_ERROR_INVAL,
	GFS_ERROR_NFILE,
	GFS_ERROR_MFILE,
	/* ENOTTY, */
	GFS_ERROR_TXTBSY,
	GFS_ERROR_FBIG,
	GFS_ERROR_NOSPC,
	GFS_ERROR_SPIPE,
	GFS_ERROR_ROFS,
	/* EMLINK, */
	GFS_ERROR_PIPE,
	/* EDOM, */
	/* ERANGE, */
	GFS_ERROR_AGAIN,
	/* : */
	GFS_ERROR_OPNOTSUPP,
	/* : */
	GFS_ERROR_CONNREFUSED,
	GFS_ERROR_LOOP,
	GFS_ERROR_NAMETOOLONG,
	/* : */
	GFS_ERROR_DQUOT,
	GFS_ERROR_STALE,
	/* : */
	GFS_ERROR_NOLCK,
	GFS_ERROR_NOSYS,
	/* EFTYPE, */
	GFS_ERROR_AUTH,
	/* : */
	GFS_ERROR_EXPIRED,
	GFS_ERROR_PROTO,
	GFS_ERROR_PROTONOSUPPORT,
	GFS_ERROR_UNKNOWN,
	/* : */
	GFS_ERROR_NUMBER
};

#define GFS_PROTO_MAX_IOSIZE	65536

/*
 * sub protocols of GFS_PROTO_COMMAND
 */

enum gfs_proto_command_client {
	GFS_PROTO_COMMAND_EXIT_STATUS,
	GFS_PROTO_COMMAND_SEND_SIGNAL,
	GFS_PROTO_COMMAND_FD_INPUT,
};

enum gfs_proto_command_server {
	GFS_PROTO_COMMAND_EXITED,
	GFS_PROTO_COMMAND_STOPPED, /* currently not used */
	GFS_PROTO_COMMAND_FD_OUTPUT,
};

#define GFARM_DEFAULT_COMMAND_IOBUF_SIZE 0x4000

#define FDESC_STDIN	0
#define FDESC_STDOUT	1
#define FDESC_STDERR	2
#define NFDESC		3

char *gfs_proto_error_string(enum gfs_proto_error);
enum gfs_proto_error gfs_string_to_proto_error(char *);
enum gfs_proto_error gfs_errno_to_proto_error(int);

#include <openssl/evp.h> /* for EVP_MD, EVP_MD_CTX */

int gfs_open_flags_localize(int);
int gfs_digest_calculate_local(int, char *, size_t,
	const EVP_MD *, EVP_MD_CTX *,
	unsigned int *, unsigned char *, file_offset_t *);
