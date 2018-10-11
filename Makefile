DIRS= ./build

all:
	@for i in $(DIRS); do \
	(cd $$i && $(MAKE)) || exit 1; \
	done

r:
	@for i in $(DIRS); do \
	(cd $$i && $(MAKE) r ) || exit 1; \
	done

clean:
	@for i in $(DIRS); do \
	(cd $$i && $(MAKE) clean ) || exit 1; \
	done

