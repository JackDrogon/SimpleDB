CC := clang
CFLAGS := -Weverything -Wno-padded
CXX := clang++
CXXFLAGS := -Weverything -std=c++11 -Wno-c++98-compat -Wno-padded -I../include -I.
LD := clang
LDFLAGS :=

AR := ar rcus

LIB := lib/simpledb.a


ifeq ($(DEBUG),yes)
	CFLAGS   += -O0 -g -pg -ggdb3
	CXXFLAGS += -O0 -g -pg -ggdb3
else
	CFLAGS   += -O3 -DNDEBUG
	CXXFLAGS += -O3 -DNDEBUG
endif
