# $Id$

top_builddir = ../..
top_srcdir = $(top_builddir)
srcdir = .

include $(top_srcdir)/makes/var.mk

bindir = $(default_libexecdir)

PROGRAM = gfrepbe_client
SRCS = $(PROGRAM).c
OBJS = $(PROGRAM).o
CFLAGS = $(COMMON_CFLAGS) -I$(GFARMLIB_SRCDIR) $(openssl_includes)
LDLIBS = $(COMMON_LDFLAGS) $(GFARMLIB) $(openssl_libs) $(LIBS)
DEPLIBS = $(DEPGFARMLIB)

all: $(PROGRAM)

include $(top_srcdir)/makes/gfptool.mk

###

$(OBJS): \
	$(GFARMLIB_SRCDIR)/xxx_proto.h \
	$(GFARMLIB_SRCDIR)/io_fd.h \
	$(GFARMLIB_SRCDIR)/param.h \
	$(GFARMLIB_SRCDIR)/sockopt.h \
	$(GFARMLIB_SRCDIR)/auth.h \
	$(GFARMLIB_SRCDIR)/gfs_proto.h \
	$(GFARMLIB_SRCDIR)/gfs_client.h \
	$(DEPGFARMINC)
