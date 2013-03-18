CC = gcc
C++ = g++
O = o
RM = rm -f
AR = ar rcs
AR2 = ranlib
CFLAGS= -g
LIBPNGNAME= libpng.a

# Lib png objects
LIBPNGOBJS= png.o       pngget.o    pngread.o   pngrutil.o  pngtrans.o  \
pngerror.o  pngmem.o    pngrio.o    pngset.o    pngvcrd.o   pngwtran.o	\
pnggccrd.o  pngpread.o  pngrtran.o  pngtest.o   pngwio.o    pngwutil.o	\
pngwrite.o

# Lib png headers 
LIBPNGHDRS= pngconf.h  png.h

$(LIBPNGNAME): $(LIBPNGOBJS) $(LIBPNGHDRS)
	$(RM) $(LIBPNGNAME)
	$(AR) $(LIBPNGNAME) $(LIBPNGOBJS)
	$(AR2) $(LIBPNGNAME)
	$(RM) $(LIBPNGOBJS)

clean:
	$(RM) *.o $(LIBPNGNAME)

