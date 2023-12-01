CC 	= g++
CXX = gcc
FLAG = -Wno-literal-suffix -Wno-deprecated -D_POSIX_PTHREAD_SEMANTICS -D_POSIX_THREAD_PRIORITY_SCHEDULING
CFLAGS=  $(FLAG)  -g -Wall -I../include  $(SYSFLAGS) -std=c++11 -Wno-unused-function -Wno-strict-overflow # -fprofile-arcs -ftest-coverage -O2 -Wno-strict-overflow # -pg

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
	rm -rf *.o *.a *.lo *.d *.gcno *.gcda unit* test*
r: clean all

main:  testmain.o
	$(CC) $(CFLAGS) -o TestMain testmain.o $(OBJS) 
cleanTest:
	rm -rf TestMain TestMain.d TestMain.o 

alltest: unitmain.o testInputReader.o testCharUtils.o testurlutil.o testHostnormalizer.o testPathNormalizer.o testUrl.o
	$(CC) $(CFLAGS) -o unitmain unitmain.o testInputReader.o testurlutil.o testHostnormalizer.o testPathNormalizer.o testUrl.o -L/usr/local/lib -lgtest -pthread -L./ -lurlparse  

# Test for sigle file just need to remove // for main function
testcharutils:testCharUtils.o
	$(CC) $(CFLAGS) -o unitcharutils testCharUtils.o -L/usr/local/lib -lgtest -pthread  -L./ -lurlparse -DTEST_MAIN_ONLY
testinputreader:testInputReader.o 
	$(CC) $(CFLAGS) -o unitinputreader testInputReader.o -L/usr/local/lib -lgtest -pthread  -L./ -lurlparse  -DTEST_MAIN_ONLY
testhostnormalizer:testHostnormalizer.o 
	$(CC) $(CFLAGS) -o unithostnormalizer testHostnormalizer.o -L/usr/local/lib -lgtest -pthread -L./ -lurlparse -DTEST_MAIN_ONLY
testpathnormalizer:testPathNormalizer.o
	$(CC) $(CFLAGS) -o unitpathnormalizer testPathNormalizer.o -L/usr/local/lib -lgtest -pthread -L./ -lurlparse -DTEST_MAIN_ONLY
testurl:testUrl.o
	$(CC) $(CFLAGS) -o uniturl testUrl.o -L/usr/local/lib -lgtest -pthread -L./ -lurlparse -DTEST_MAIN_ONLY
testnormalizedurl:testNormalizedUrl.o
	$(CC) $(CFLAGS) -o unitnormalizedurl testNormalizedUrl.o -L/usr/local/lib -lgtest -pthread -L./ -lurlparse -DTEST_MAIN_ONLY
testurlutil:testurlutil.o
	$(CC) $(CFLAGS) -o uniturlutil testurlutil.o -L/usr/local/lib -lgtest -pthread -L./ -lurlparse -DTEST_MAIN_ONLY

# file reader to parse url
urlclient:urlparseclient.o
	$(CC) $(CFLAGS) -o client urlparseclient.o -L./ -lurlparse 

## Commands to generate dependency files
#GEN_DEPS.c= $(CC) -E -MM $(DEFS) $(CFLAGS)
#GEN_DEPS.cc= $(CXX) -E -MM $(DEFS) $(CFLAGS)
#%.d : %.c
#	@echo "generating dependency information for $<"
#	@$(SHELL) -ec '$(GEN_DEPS.c) $< \
#			| sed '\''s/\($*\)\.o[ :]*/\1.o $@ : /g'\'' > $@; \
#            [ -s $@ ] || rm -f $@'
#%.d : %.cpp
#	@echo "generating dependency information for $<"
#	@$(SHELL) -ec '$(GEN_DEPS.cc) $< \
#			| sed '\''s/\($*\)\.o[ :]*/\1.o $@ : /g'\'' > $@; \
#            [ -s $@ ] || rm -f $@'
＃OBJSDEP = $(OBJS:.o=.d)
#ifeq (,$(NAJECNDGLOBALS))
#-include $(OBJSDEP)
#else
#ifneq ($(%clean,,$(MAKECMDGOBALS)),)
#-include $(OBJSDEP)
#endif
#endif
