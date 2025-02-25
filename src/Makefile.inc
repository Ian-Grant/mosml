# Unix configuration Makefile for Moscow ML          -*- mode: makefile -*-

# Where to install stuff
PREFIX=/usr/local

# BINDIR contains true executable files, such as scripts
# LIBDIR contains bytecode files (such as mosmlcmp and library units), and .dll/.so for dynlibs.
#        RPATH is set to include LIBDIR
# INCDIR contains the runtime system header files (for compiling dynlibs)
# DOCDIR contains documentation

BINDIR=${PREFIX}/bin
LIBDIR=${PREFIX}/lib/mosml
INCDIR=${PREFIX}/include/mosml
DOCDIR=${PREFIX}/share/doc/mosml
TOOLDIR=${PREFIX}/share/mosml/tools

### Where to install the man pages
# Man pages for commands go in $(MANDIR)/man$(MANEXT)
MANDIR=${PREFIX}/man
MANEXT=1


# The version
VERSION=2.10

# Various utility programs
INSTALL_PROGRAM=cp
INSTALL_SCRIPT=cp
INSTALL_DATA=cp
UNINSTALL_PROGRAM=rm -f
UNINSTALL_SCRIPT=rm -f
UNINSTALL_DATA=rm -f
PERL=perl

BASELIBS=-lm

# This works with most systems, including MacOS X with XCode installed:

CC=egcc
# CC=gcc -mmacosx-version-min=10.7 # for building OS X package
# CC=/usr/sepp/bin/gcc		# Solaris at KVL

# To compile the runtime system camlrunm with support for
# dynamically loadable libraries (DDLs), set DYNLIBSUPPORT to true
DYNLIBSUPPORT=true

ifeq ($(DYNLIBSUPPORT),true)
  ADDPRIMS=dynlib.c
  ADDOBJS=dynlib.o
endif

ADDRUNLIBS=-ldl
ADDRUNCFLAGS=-fPIC
ADDDYNLIBCFLAGS=-fPIC


# Automatic detection of OS
# If you need to override the detection, set UNAME_S to Custom and add your configuration there.
#
UNAME_S := $(shell sh -c 'uname -s 2>/dev/null || echo not')


# We choose to avoid "if .. else if .. else .. endif endif"
# because maintaining the nesting to match is a pain.  If
# we had "elif" things would have been much nicer...

ifeq ($(UNAME_S),Linux)
	CPP=cpp -P -traditional -Dunix -Umsdos -Wno-invalid-pp-token
	STRIP=strip -S
	LD=$(CC) -rdynamic -Wl,-rpath,$(LIBDIR)
	DYNLD=$(CC) -shared
endif
ifeq ($(UNAME_S),Darwin) 	# For MacOS X, use the same as Linux except DYNDL
	CPP=cpp -P -traditional -Dunix -Umsdos -Wno-invalid-pp-token
	STRIP=strip -S
	LD=$(CC) -rdynamic -Wl,-rpath,$(LIBDIR)
	DYNLD=$(LD) -bundle -undefined dynamic_lookup
endif
ifeq ($(UNAME_S),Cross_W32)
	CC=i586-mingw32msvc-gcc
	CPP=cpp -P -traditional -Umsdos -Uunix -Dwin32 -Wno-invalid-pp-token
	ADDRUNLIBS=
	ADDRUNCFLAGS=
	ADDDYNLIBCFLAGS=
	STRIP=strip -S
	EXE=.exe
	LD=$(CC)
	DYNLD=$(CC) -shared
endif
ifeq ($(UNAME_S),OpenBSD)
	ADDRUNLIBS=
	CPP=cpp -P -traditional -Dunix -Umsdos -Wno-invalid-pp-token
	STRIP=strip -S
	LD=$(CC) -rdynamic -Wl,-rpath,$(LIBDIR)
	DYNLD=$(CC) -shared
endif

ifeq ($(UNAME_S),Custom) # Your configuration here
error:
	@echo "Unknown OS $(UNAME_S), please update Makefile.inc and send a patch to mosml.org"
endif

# Note for contributers, please check and convert the following to your platform of choice
# For FreeBSD, use:
# -----------------
# CPP=/usr/bin/cpp -P -traditional -Dunix -Umsdos
# STRIP=strip
# LD=gcc -export-dynamic -Wl,-rpath,$(LIBDIR)
# DYNLD=ld -shared
# ADDRUNLIBS=


# For Cygwin 20.1, use:
# ---------------------
# MOSMLHOME=/mosml
# CPP=/lib/cpp -P -traditional -Dunix -Umsdos
# STRIP=strip
# LD=gcc -Wl,--export-dynamic -Wl,-rpath,$(LIBDIR)
# DYNLD=ld -shared
# EXE=.exe
# # For dynamically loadable libraries (DDLs) under Cygwin:
# ADDPRIMS=dynlib.c
# ADDOBJS=dynlib.o
# ADDRUNCFLAGS=
# ADDDYNLIBCFLAGS=

# For NetBSD, use:
# ------------------------
# CPP=/usr/bin/cpp -P -traditional -Dunix -Umsdos
# STRIP=/usr/bin/strip
# ADDRUNCFLAGS=-fPIC
# ADDDYNLIBCFLAGS=-fPIC
# INSTALL_PROGRAM=${BSD_INSTALL_PROGRAM}
# INSTALL_SCRIPT=${BSD_INSTALL_SCRIPT}
# INSTALL_DATA=${BSD_INSTALL_DATA}

# For Solaris 2 (not SunOS 4), use:
# ---------------------------------
# CPP=/usr/ccs/lib/cpp -P -Dunix -Umsdos
# STRIP=/usr/ccs/bin/strip
# LD=gcc
# DYNLD=ld -G -B symbolic -z nodefs
# ADDRUNCFLAGS=
# ADDDYNLIBCFLAGS=

# If the above fails with dynlibs, try this:
# LD=gcc -Wl,--export-dynamic -Wl,-rpath,$(LIBDIR)


# For HP-UX 9, use:
# -----------------
# CPP=/lib/cpp -P -Dunix -Umsdos
# STRIP=strip
# LD=gcc -Xlinker -E
# DYNLD=ld -b -B symbolic -E
# ADDRUNLIBS=-ldld
# ADDRUNCFLAGS=-fPIC
# ADDDYNLIBCFLAGS=-fPIC

# For HP-UX 10, use:
# ------------------
# CPP=/opt/langtools/lbin/cpp -P -Dunix -Umsdos
# STRIP=strip
# LD=gcc -Xlinker -E
# DYNLD=ld -b -B symbolic -E
# ADDRUNLIBS=-ldld
# ADDRUNCFLAGS=-fPIC
# ADDDYNLIBCFLAGS=-fPIC

# For OSF/1 (Digital Unix), use:
# ------------------------------
# CPP=/lib/cpp -P -traditional -Dunix -Umsdos
# STRIP=strip
# LD=gcc
# DYNLD=ld -shared
# ADDRUNLIBS=
# ADDCFLAGS=
# ADDRUNCFLAGS=
# ADDDYNLIBCFLAGS=

# For Irix, use:
# --------------
# CPP=/lib/cpp -P -Dunix -Umsdos
# STRIP=/usr/bin/strip
# LD=gcc -rdynamic
# DYNLD=ld -shared

SHELL=/bin/sh

# ---------- You shouldn't need to edit below this line ------------
ROOTDIR:=..

CAMLRT=$(ROOTDIR)/runtime

MOSMLTOOLS=$(ROOTDIR)/tools

MOSMLC=$(ROOTDIR)/camlrunm $(ROOTDIR)/mosmlcmp -stdlib $(ROOTDIR)/mosmllib -P none
MOSMLL=$(ROOTDIR)/camlrunm $(ROOTDIR)/mosmllnk -stdlib $(ROOTDIR)/mosmllib -P none
MOSMLLDOS=$(ROOTDIR)/camlrunm $(ROOTDIR)/mosmllnk.dos -stdlib $(ROOTDIR)/mosmllib -P none
MOSMLLW32=$(ROOTDIR)/camlrunm $(ROOTDIR)/mosmllnk.w32 -stdlib $(ROOTDIR)/mosmllib -P none
MOSMLLNEW=$(ROOTDIR)/camlrunm $(ROOTDIR)/mosmllnk.new -stdlib $(ROOTDIR)/mosmllib -P none

MOSMLLEX=$(ROOTDIR)/camlrunm $(ROOTDIR)/mosmllex
MOSMLYACC=$(ROOTDIR)/mosmlyac/mosmlyac
MOSMLCUT=$(ROOTDIR)/camlrunm $(ROOTDIR)/cutdeps
MOSMLDEP=$(ROOTDIR)/camlrunm $(ROOTDIR)/mosmldep

# For cross-compiling to MS DOS (from Linux) (development only)
# CPP=/lib/cpp -P -traditional -Uunix -Dmsdos -Uwin32

# For cross-compiling to Win 32 (from Linux) (development only)
# CPP=/lib/cpp -P -traditional -Umsdos -Uunix -Dwin32

.SUFFIXES :
.SUFFIXES : .sml .sig .ui .uo .mlp .lex .c .o

.sig.ui:
	$(MOSMLC) $(COMPFLAGS) $<

.sml.uo:
	$(MOSMLC) $(COMPFLAGS) $<

.mlp.sml:
	$(CPP) $< > $*.sml

.lex.sml:
	$(MOSMLLEX) $<

.c.o:
	$(CC) -c $(CFLAGS) -o $@ $<
