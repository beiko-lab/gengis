CC = gcc
C++ = g++
O = o
RM = rm -f
AR = ar rcs
AR2 = ranlib
CFLAGS= -g
LIBJPEGNAME= libjpeg.a

# Lib jpeg objects
LIBJPGOBJS= jcapimin.o  jcmarker.o  jdapimin.o  jdinput.o   jdtrans.o 	\
jcapistd.o  jcmaster.o  jdapistd.o  jdmainct.o  jerror.o    jmemansi.o 	\
jccoefct.o  jcomapi.o   jdatadst.o  jdmarker.o  jfdctflt.o  jmemmgr.o 	\
jccolor.o   jcparam.o   jdatasrc.o  jdmaster.o  jfdctfst.o  jquant1.o   \
jcdctmgr.o  jcphuff.o   jdcoefct.o  jdmerge.o   jfdctint.o  jquant2.o 	\
jchuff.o    jcprepct.o  jdcolor.o   jdphuff.o   jidctflt.o  jutils.o	\
jcinit.o    jcsample.o  jddctmgr.o  jdpostct.o  jidctfst.o  jidctred.o	\
jcmainct.o  jctrans.o   jdhuff.o    jdsample.o  jidctint.o

# Lib jpeg headers 
LIBJPGHDRS= jchuff.h   jdct.h    jerror.h    jmemsys.h   jpegint.h	\
jversion.h  jconfig.h  jdhuff.h  jinclude.h  jmorecfg.h  jpeglib.h

$(LIBJPEGNAME): $(LIBJPGOBJS) $(LIBJPGHDRS)
	$(RM) $(LIBJPEGNAME)
	$(AR) $(LIBJPEGNAME) $(LIBJPGOBJS)
	$(AR2) $(LIBJPEGNAME)
	$(RM) $(LIBJPGOBJS)

clean:
	$(RM) *.o $(LIBJPEGNAME)

