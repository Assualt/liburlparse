CC 	= g++
CXX = gcc
FLAG = -Wno-literal-suffix -Wno-deprecated -D_POSIX_PTHREAD_SEMANTICS -D_POSIX_THREAD_PRIORITY_SCHEDULING
CFLAGS 	= $(FLAG)  -g -Wall -I../include -O3 $(SYSFLAGS) -std=c++11
#VPATH 指定cpp文件的路径
VPATH = ../src:../test
#LIBS 指定链接文件库
OBJS = stringutils.o stringbuilder.o inetaddress.o charutils.o domainnamereader.o \
       inputtextreader.o urldetector.o hostnormalizer.o url.o urlmarker.o pathnormalizer.o \
       urlutil.o urlpart.o urldetectoroptions.o glog.o
.cpp.o:
	$(CC) -c $(CFLAGS) $(OPT) $<
.cc.o:
	$(CC) -c $(CFLAGS) $(OPT) $<
liburlparse.a: $(OBJS)
	rm -f liburlparse.a
	ar cr liburlparse.a $(OBJS)
	ranlib liburlparse.a #更新符号表
all: liburlparse.a
clean: 
	rm -rf *.o *.a *.lo *.d
r: clean all
# TEST
TestMain:  testmain.o
	$(CC) $(CFLAGS) -o TestMain testmain.o $(OBJS) 
cleanTest:
	rm -rf TestMain TestMain.d TestMain.o
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
