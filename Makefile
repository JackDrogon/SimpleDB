.PHONY: clean
.PHONY: all

all: lib

lib:
	$(MAKE) -C src

test: lib

clean:
	-$(MAKE) -C src clean
