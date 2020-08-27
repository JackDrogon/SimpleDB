-include ../define.mk

CXXFLAGS +=  -I../include -I.
LIB := ../$(LIB)

BASIC_OBJS := db/db_impl.o
UTILS_OBJS := utils/coding.o
FILE_OBJS := file/record_writer.o file/file.o

.PHONY: clean
.PHONY: all

# TODO debug

all: $(LIB)

$(LIB): $(BASIC_OBJS) $(FILE_OBJS) $(UTILS_OBJS)
	mkdir -p ../lib/
	$(AR) $@ $^

clean:
	-$(RM) $(BASIC_OBJS) $(UTILS_OBJS) $(FILE_OBJS) $(LIB)
	-$(RM) -rf ../lib
