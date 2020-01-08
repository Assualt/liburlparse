CC 	= g++
CXX = gcc
FLAG = -Wno-literal-suffix -Wno-deprecated -D_POSIX_PTHREAD_SEMANTICS -D_POSIX_THREAD_PRIORITY_SCHEDULING
CFLAGS 	= $(FLAG)  -g -Wall -I../include  $(SYSFLAGS) -std=c++11 -fprofile-arcs -ftest-coverage 
#VPATH 指定cpp文件的路径
VPATH = ../src:../Test
#LIBS 指定链接文件库
OBJS = stringutils.o inetaddress.o charutils.o domainnamereader.o \
       inputtextreader.o urldetector.o hostnormalizer.o url.o urlmarker.o pathnormalizer.o \
       urlutil.o urlpart.o urldetectoroptions.o glog.o
.cpp.o:
	$(CC) -c $(CFLAGS) $(OPT) $<
.cc.o:
	$(CC) -c $(CFLAGS) $(OPT) $<
liburlparse.a: $(OBJS)
	rm -f liburlparse.a
	ar cr liburlparse.a $(OBJS)
	ranlib liburlparse.a
all: liburlparse.a
clean: 
	rm -rf *.o *.a *.lo *.d *.gcno *.gcda
r: clean all

main:  testmain.o
	$(CC) $(CFLAGS) -o TestMain testmain.o $(OBJS) 
cleanTest:
	rm -rf TestMain TestMain.d TestMain.o

alltest: unitmain.o testInputReader.o testCharUtils.o testurlutil.o
	$(CC) $(CFLAGS) -o unitmain unitmain.o testInputReader.o testurlutil.o -L/usr/local/lib -lgtest -pthread -L./ -lurlparse  

testcharutils:testCharUtils.o
	$(CC) $(CFLAGS) -o unitcharutils testCharUtils.o -L/usr/local/lib -lgtest -pthread -L./ -lurlparse 
testinputreader:testInputReader.o
	$(CC) $(CFLAGS) -o unitinputreader testInputReader.o -L/usr/local/lib -lgtest -pthread -L./ -lurlparse

urlclient:urlparseclient.o
	$(CC) $(CFLAGS) -o client urlparseclient.o -L./ -lurlparse 

testhostnormalizer:testHostnormalizer.o
	$(CC) $(CFLAGS) -o unithostnormalizer testHostnormalizer.o -L/usr/local/lib -lgtest -pthread -L./ -lurlparse
## Commands to generate dependency files
GEN_DEPS.c= $(CC) -E -MM $(DEFS) $(CFLAGS)
GEN_DEPS.cc= $(CXX) -E -MM $(DEFS) $(CFLAGS)
%.d : %.c
	@echo "generating dependency information for $<"
	@$(SHELL) -ec '$(GEN_DEPS.c) $< \
			| sed '\''s/\($*\)\.o[ :]*/\1.o $@ : /g'\'' > $@; \
            [ -s $@ ] || rm -f $@'
%.d : %.cpp
	@echo "generating dependency information for $<"
	@$(SHELL) -ec '$(GEN_DEPS.cc) $< \
			| sed '\''s/\($*\)\.o[ :]*/\1.o $@ : /g'\'' > $@; \
            [ -s $@ ] || rm -f $@'
OBJSDEP = $(OBJS:.o=.d)
ifeq (,$(NAJECNDGLOBALS))
-include $(OBJSDEP)
else
ifneq ($(%clean,,$(MAKECMDGOBALS)),)
-include $(OBJSDEP)
endif
endif
