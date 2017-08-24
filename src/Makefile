-include ../define.mk

LIB := ../$(LIB)

BASIC_OBJS := db/db.o
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
	-$(RM) $(DB_TEST_OBJ) $(BASIC_OBJS) $(UTILS_OBJS) $(LIB)
