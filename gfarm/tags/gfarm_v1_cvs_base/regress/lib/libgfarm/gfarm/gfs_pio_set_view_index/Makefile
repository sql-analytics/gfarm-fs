top_builddir = ../../../../..
top_srcdir = $(top_builddir)
srcdir = .

include $(top_srcdir)/makes/var.mk

PROGRAM = set_view_index
SRCS = $(PROGRAM).c
OBJS = $(PROGRAM).o
CFLAGS = $(COMMON_CFLAGS)
LDLIBS = $(COMMON_LDLIBS) $(GFARMLIB) $(LIBS)
DEPLIBS = $(DEPGFARMLIB)

all: $(PROGRAM)

include $(top_srcdir)/makes/prog.mk

###

$(OBJS): $(DEPGFARMINC)
