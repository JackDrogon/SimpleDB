CC := clang
CFLAGS := -Weverything -ggdb -g
CXX := clang++
CXXFLAGS := -Weverything -ggdb -g -std=c++11
LD := clang
LDFLAGS :=

all: db_test
.PHONY: clean
.PHONY: all

db_test: db_test.o db.o
	$(CXX) $(CXXFLAGS) -o $@ $^

clean:
	-$(RM) *.o
	-$(RM) db_test