CC := clang
CFLAGS := -Weverything -ggdb -g
CXX := clang++
CXXFLAGS := -Weverything -O0 -ggdb -g -std=c++11 -Wno-c++98-compat
LD := clang
LDFLAGS :=

all: db_test
.PHONY: clean
.PHONY: all

# TODO debug

db_test: db_test.o db.o coding.o file.o
	$(CXX) $(CXXFLAGS) -o $@ $^

clean:
	-$(RM) *.o
	-$(RM) db_test
