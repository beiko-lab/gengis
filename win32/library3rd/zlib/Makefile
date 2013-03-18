CC = gcc
C++ = g++
O = o
RM = rm -f
AR = ar rcs
AR2 = ranlib
CFLAGS= -g
LIBZLIBNAME= libzlib.a

# Lib zlib objects
LIBZLIBOBJS= adler32.o   crc32.o    gzio.o     inffast.o  inftrees.o  \
uncompr.o  compress.o  deflate.o  infback.o  inflate.o  trees.o   \
zutil.o


# Lib zlib headers 
LIBZLIBHDRS= crc32.h  inffast.h   inflate.h   trees.h  zlib.h \
deflate.h  inffixed.h  inftrees.h  zconf.h  zutil.h

$(LIBZLIBNAME): $(LIBZLIBOBJS) $(LIBZLIBHDRS)
	$(RM) $(LIBZLIBNAME)
	$(AR) $(LIBZLIBNAME) $(LIBZLIBOBJS)
	$(AR2) $(LIBZLIBNAME)
	$(RM) $(LIBZLIBOBJS)

clean:
	$(RM) *.o $(LIBZLIBNAME)

