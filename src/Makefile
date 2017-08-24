CC := clang
CFLAGS := -Weverything -ggdb -g
CXX := clang++
CXXFLAGS := -Weverything -O0 -ggdb -g -std=c++11 -Wno-c++98-compat -I../include -I.
LD := clang
LDFLAGS :=

AR := ar rcus

BASIC_OBJS := db/db.o
UTILS_OBJS := utils/coding.o
FILE_OBJS := file/record_writer.o file/file.o
LIB := ../lib/simpledb.a

.PHONY: clean
.PHONY: all

# TODO debug

all: $(LIB)

$(LIB): $(BASIC_OBJS) $(FILE_OBJS) $(UTILS_OBJS)
	mkdir -p ../lib/
	$(AR) $@ $^

clean:
	-$(RM) $(DB_TEST_OBJ) $(BASIC_OBJS) $(UTILS_OBJS) $(LIB)
