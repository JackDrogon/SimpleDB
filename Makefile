-include define.mk

.PHONY: clean
.PHONY: all



all: $(LIB)

$(LIB):
	$(MAKE) -C src

test: lib

clean:
	-$(MAKE) -C src clean
