CC := clang
CFLAGS := -Weverything -ggdb -g
CXX := clang++
CXXFLAGS := -Weverything -O0 -ggdb -g -std=c++11 -Wno-c++98-compat
LD := clang
LDFLAGS :=

BASIC_OBJS := db.o coding.o record_writer.o file.o
DB_TEST := db_test
DB_TEST_OBJ := db_test.o

all: db_test
.PHONY: clean
.PHONY: all

# TODO debug

$(DB_TEST): $(DB_TEST_OBJ) $(BASIC_OBJS)
	$(CXX) $(CXXFLAGS) -o $@ $^

clean:
	-$(RM) $(DB_TEST) $(DB_TEST_OBJ) $(BASIC_OBJS)
	-$(RM) db_test
