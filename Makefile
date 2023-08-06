# Makefile for tcanetpp
#  Requires 'tcamake' build environment to exist in TOPDIR
#
NEED_SOCKET = 1
NEED_LIBDL = 1

ifdef USE_LIBRT
NEED_LIBRT = 1
endif
ifdef USE_PTHREADS
NEED_PTHREADS = 1
endif

#-------------------#

ifdef TCAMAKE_DEBUG
  OPT_FLAGS =       -g -DEV_DEBUG
endif

OPT_FLAGS +=	    -fPIC -O2
CCSHARED += 	    -Wl,-soname,$@
CXXFLAGS =          -std=c++11

INCLUDES =          -Iinclude
LIBS =

PT_OBJS =           src/patricia.o
TH_OBJS =           src/Thread.o src/ThreadLock.o src/ThreadMutexPool.o
CMDBUF_OBJS =       src/CmdBuffer.o

OBJS =	            src/SocketOption.o src/Socket.o src/BufferedSocket.o \
                    src/CircularBuffer.o src/Serializer.o \
                    src/Whois.o src/EventManager.o \
                    src/IpAddr.o src/AddrInfo.o \
                    src/StringUtils.o src/FileStat.o src/FileUtils.o \
                    src/LogFacility.o src/tcanetpp_random.o src/INotify.o \
                    src/patricia.o src/DeviceMap.o \
                    src/NetworkDevice.o src/NetworkInterface.o

ifdef USE_PTHREADS
OBJS +=		    $(TH_OBJS)
endif

BIN =
ALL_OBJS =	    $(OBJS) $(TH_OBJS) $(PT_OBJS) $(CMDBUF_OBJS)
ALL_BINS = 	    $(BIN)

# ---------------------------------------------

include ${TCAMAKE_HOME}/tcamake_include

# ---------------------------------------------

all: lib cmdbuf
lib: arlib

arlib: lib/libtcanetpp.a
solib: libtcanetpp.so.1.4.4
libtcapt: lib/libtcapt.a

cmdbuffer: cmdbuf
cmdbuf:    libcmdbuf
libcmdbuf: lib/libcmdbuf.a

libtcanetpp.so.1.4.4: ${OBJS}
	( $(MKDIR) lib )
	( $(RM) $@ lib/libtcanetpp.so )
	$(make-so-rule)
	( mv $@ lib/; cd lib; ln -s $@ libtcanetpp.so )
	@echo

lib/libtcanetpp.a: ${OBJS}
	( $(MKDIR) lib )
	$(make-lib-rule)
	@echo

lib/libtcapt.a: ${PT_OBJS}
	( $(MKDIR) lib )
	$(make-lib-rule)
	@echo

lib/libcmdbuf.a: ${CMDBUF_OBJS}
	( $(MKDIR) lib )
	$(make-lib-rule)
	@echo

documentation:
	(cd docs; ${MAKE} ${MFLAGS} ${MVARS} all )
	@echo

libclean:
	rm -rf lib
	@echo

doc-clean:
	( cd docs; $(MAKE) clean )
	@echo

.PHONY: test
test:
	( cd test; $(MAKE) all )
	@echo 

clean:
	$(RM) $(ALL_OBJS) \
	src/*.d src/*.D src/*.bd src/*.o lib/*.bd *.bd
	@echo

testclean: test-clean
test-clean:
	( cd test; $(MAKE) distclean )
	@echo

distclean: clean libclean doc-clean test-clean
	$(RM) $(ALL_BINS)
	@echo

dist: distclean
ifdef TCAMAKE_DISTDIR
	@echo "Sync distribution to $(TCAMAKE_DISTDIR)/tcanetpp"
	( $(TOPDIR)/tcamake/scripts/tcamake_build.sh dist $(TCAMAKE_DISTDIR) )
	@echo
endif

install: clean
ifdef TCAMAKE_PREFIX
	@echo "Installing libtcanetpp to $(TCAMAKE_PREFIX)/{include,lib}"
	$(MKDIR) $(TCAMAKE_PREFIX)/include/tcanetpp
	$(MKDIR) $(TCAMAKE_PREFIX)/lib
	$(RSYNC) --delete include/ $(TCAMAKE_PREFIX)/include/tcanetpp/
	$(RSYNC) lib/ $(TCAMAKE_PREFIX)/lib/
	@echo
else
	@echo "TCAMAKE_PREFIX is not set. Install not performed"
	@echo "  eg. export TCAMAKE_PREFIX=/usr/local"
endif
