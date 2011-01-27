###################################################
#
# Makefile for MCMapper
# Creator [Xcode -> Makefile Ver: Feb 14 2007 09:18:41]
# Created: [Thu Jan 27 08:46:25 2011]
#
###################################################

#
# Macros
#

CC = /usr/bin/gcc
CC_OPTIONS = 
LNK_OPTIONS = -lStdC++


#
# INCLUDE directories for MCMapper
#

INCLUDE = -I.\
		-Ilibpng\
		-Izlib\
		-Ipngwriter


#
# Build MCMapper
#

MCMapper : \
		build/main.o\
		build/base64.o\
		build/png.o\
		build/pngerror.o\
		build/pngget.o\
		build/pngmem.o\
		build/pngpread.o\
		build/pngread.o\
		build/pngrio.o\
		build/pngrtran.o\
		build/pngrutil.o\
		build/pngset.o\
		build/pngtrans.o\
		build/pngwio.o\
		build/pngwrite.o\
		build/pngwtran.o\
		build/pngwutil.o\
		build/adler32.o\
		build/compress.o\
		build/crc32.o\
		build/deflate.o\
		build/gzclose.o\
		build/gzlib.o\
		build/gzread.o\
		build/gzwrite.o\
		build/infback.o\
		build/inffast.o\
		build/inflate.o\
		build/inftrees.o\
		build/trees.o\
		build/uncompr.o\
		build/zutil.o\
		build/pngwriter.o
	$(CC) $(LNK_OPTIONS) \
		build/main.o\
		build/base64.o\
		build/png.o\
		build/pngerror.o\
		build/pngget.o\
		build/pngmem.o\
		build/pngpread.o\
		build/pngread.o\
		build/pngrio.o\
		build/pngrtran.o\
		build/pngrutil.o\
		build/pngset.o\
		build/pngtrans.o\
		build/pngwio.o\
		build/pngwrite.o\
		build/pngwtran.o\
		build/pngwutil.o\
		build/adler32.o\
		build/compress.o\
		build/crc32.o\
		build/deflate.o\
		build/gzclose.o\
		build/gzlib.o\
		build/gzread.o\
		build/gzwrite.o\
		build/infback.o\
		build/inffast.o\
		build/inflate.o\
		build/inftrees.o\
		build/trees.o\
		build/uncompr.o\
		build/zutil.o\
		build/pngwriter.o\
		-o MCMapper

clean : 
		rm \
		build/main.o\
		build/base64.o\
		build/png.o\
		build/pngerror.o\
		build/pngget.o\
		build/pngmem.o\
		build/pngpread.o\
		build/pngread.o\
		build/pngrio.o\
		build/pngrtran.o\
		build/pngrutil.o\
		build/pngset.o\
		build/pngtrans.o\
		build/pngwio.o\
		build/pngwrite.o\
		build/pngwtran.o\
		build/pngwutil.o\
		build/adler32.o\
		build/compress.o\
		build/crc32.o\
		build/deflate.o\
		build/gzclose.o\
		build/gzlib.o\
		build/gzread.o\
		build/gzwrite.o\
		build/infback.o\
		build/inffast.o\
		build/inflate.o\
		build/inftrees.o\
		build/trees.o\
		build/uncompr.o\
		build/zutil.o\
		build/pngwriter.o\
		MCMapper

install : MCMapper
		cp MCMapper MCMapper

#
# Build the parts of MCMapper
#


# Item # 1 -- main --
build/main.o : main.cpp
	$(CC) $(CC_OPTIONS) main.cpp -c $(INCLUDE) -o build/main.o


# Item # 2 -- base64 --
build/base64.o : base64.cpp
	$(CC) $(CC_OPTIONS) base64.cpp -c $(INCLUDE) -o build/base64.o


# Item # 3 -- png --
build/png.o : libpng/png.c
	$(CC) $(CC_OPTIONS) libpng/png.c -c $(INCLUDE) -o build/png.o


# Item # 4 -- pngerror --
build/pngerror.o : libpng/pngerror.c
	$(CC) $(CC_OPTIONS) libpng/pngerror.c -c $(INCLUDE) -o build/pngerror.o


# Item # 5 -- pngget --
build/pngget.o : libpng/pngget.c
	$(CC) $(CC_OPTIONS) libpng/pngget.c -c $(INCLUDE) -o build/pngget.o


# Item # 6 -- pngmem --
build/pngmem.o : libpng/pngmem.c
	$(CC) $(CC_OPTIONS) libpng/pngmem.c -c $(INCLUDE) -o build/pngmem.o


# Item # 7 -- pngpread --
build/pngpread.o : libpng/pngpread.c
	$(CC) $(CC_OPTIONS) libpng/pngpread.c -c $(INCLUDE) -o build/pngpread.o


# Item # 8 -- pngread --
build/pngread.o : libpng/pngread.c
	$(CC) $(CC_OPTIONS) libpng/pngread.c -c $(INCLUDE) -o build/pngread.o


# Item # 9 -- pngrio --
build/pngrio.o : libpng/pngrio.c
	$(CC) $(CC_OPTIONS) libpng/pngrio.c -c $(INCLUDE) -o build/pngrio.o


# Item # 10 -- pngrtran --
build/pngrtran.o : libpng/pngrtran.c
	$(CC) $(CC_OPTIONS) libpng/pngrtran.c -c $(INCLUDE) -o build/pngrtran.o


# Item # 11 -- pngrutil --
build/pngrutil.o : libpng/pngrutil.c
	$(CC) $(CC_OPTIONS) libpng/pngrutil.c -c $(INCLUDE) -o build/pngrutil.o


# Item # 12 -- pngset --
build/pngset.o : libpng/pngset.c
	$(CC) $(CC_OPTIONS) libpng/pngset.c -c $(INCLUDE) -o build/pngset.o


# Item # 13 -- pngtrans --
build/pngtrans.o : libpng/pngtrans.c
	$(CC) $(CC_OPTIONS) libpng/pngtrans.c -c $(INCLUDE) -o build/pngtrans.o


# Item # 14 -- pngwio --
build/pngwio.o : libpng/pngwio.c
	$(CC) $(CC_OPTIONS) libpng/pngwio.c -c $(INCLUDE) -o build/pngwio.o


# Item # 15 -- pngwrite --
build/pngwrite.o : libpng/pngwrite.c
	$(CC) $(CC_OPTIONS) libpng/pngwrite.c -c $(INCLUDE) -o build/pngwrite.o


# Item # 16 -- pngwtran --
build/pngwtran.o : libpng/pngwtran.c
	$(CC) $(CC_OPTIONS) libpng/pngwtran.c -c $(INCLUDE) -o build/pngwtran.o


# Item # 17 -- pngwutil --
build/pngwutil.o : libpng/pngwutil.c
	$(CC) $(CC_OPTIONS) libpng/pngwutil.c -c $(INCLUDE) -o build/pngwutil.o


# Item # 18 -- adler32 --
build/adler32.o : zlib/adler32.c
	$(CC) $(CC_OPTIONS) zlib/adler32.c -c $(INCLUDE) -o build/adler32.o


# Item # 19 -- compress --
build/compress.o : zlib/compress.c
	$(CC) $(CC_OPTIONS) zlib/compress.c -c $(INCLUDE) -o build/compress.o


# Item # 20 -- crc32 --
build/crc32.o : zlib/crc32.c
	$(CC) $(CC_OPTIONS) zlib/crc32.c -c $(INCLUDE) -o build/crc32.o


# Item # 21 -- deflate --
build/deflate.o : zlib/deflate.c
	$(CC) $(CC_OPTIONS) zlib/deflate.c -c $(INCLUDE) -o build/deflate.o


# Item # 22 -- gzclose --
build/gzclose.o : zlib/gzclose.c
	$(CC) $(CC_OPTIONS) zlib/gzclose.c -c $(INCLUDE) -o build/gzclose.o


# Item # 23 -- gzlib --
build/gzlib.o : zlib/gzlib.c
	$(CC) $(CC_OPTIONS) zlib/gzlib.c -c $(INCLUDE) -o build/gzlib.o


# Item # 24 -- gzread --
build/gzread.o : zlib/gzread.c
	$(CC) $(CC_OPTIONS) zlib/gzread.c -c $(INCLUDE) -o build/gzread.o


# Item # 25 -- gzwrite --
build/gzwrite.o : zlib/gzwrite.c
	$(CC) $(CC_OPTIONS) zlib/gzwrite.c -c $(INCLUDE) -o build/gzwrite.o


# Item # 26 -- infback --
build/infback.o : zlib/infback.c
	$(CC) $(CC_OPTIONS) zlib/infback.c -c $(INCLUDE) -o build/infback.o


# Item # 27 -- inffast --
build/inffast.o : zlib/inffast.c
	$(CC) $(CC_OPTIONS) zlib/inffast.c -c $(INCLUDE) -o build/inffast.o


# Item # 28 -- inflate --
build/inflate.o : zlib/inflate.c
	$(CC) $(CC_OPTIONS) zlib/inflate.c -c $(INCLUDE) -o build/inflate.o


# Item # 29 -- inftrees --
build/inftrees.o : zlib/inftrees.c
	$(CC) $(CC_OPTIONS) zlib/inftrees.c -c $(INCLUDE) -o build/inftrees.o


# Item # 30 -- trees --
build/trees.o : zlib/trees.c
	$(CC) $(CC_OPTIONS) zlib/trees.c -c $(INCLUDE) -o build/trees.o


# Item # 31 -- uncompr --
build/uncompr.o : zlib/uncompr.c
	$(CC) $(CC_OPTIONS) zlib/uncompr.c -c $(INCLUDE) -o build/uncompr.o


# Item # 32 -- zutil --
build/zutil.o : zlib/zutil.c
	$(CC) $(CC_OPTIONS) zlib/zutil.c -c $(INCLUDE) -o build/zutil.o


# Item # 33 -- pngwriter --
build/pngwriter.o : pngwriter/pngwriter.cc
	$(CC) $(CC_OPTIONS) pngwriter/pngwriter.cc -c $(INCLUDE) -o build/pngwriter.o


##### END RUN ####
