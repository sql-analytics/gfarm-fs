# $Id: Makefile 2533 2006-03-31 10:50:56Z takuya $

top_builddir = ../../..
top_srcdir = $(top_builddir)
srcdir = .

include $(top_srcdir)/makes/var.mk

CFLAGS = $(COMMON_CFLAGS) -I$(GFUTIL_SRCDIR) -I$(GFARMLIB_SRCDIR) \
	-I$(srcdir)/../gfperf-lib $(openssl_includes)
LDLIBS = $(COMMON_LDFLAGS) -L../gfperf-lib $(GFARMLIB) $(LIBS) -lgfperf
DEPLIBS = $(DEPGFARMLIB)

PROGRAM = gfperf-replica
OBJS = gfperf-replica-main.o gfperf-replica-test.o

all: $(PROGRAM)

include $(top_srcdir)/makes/prog.mk
