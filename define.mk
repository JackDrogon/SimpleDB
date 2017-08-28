CC := clang
CFLAGS := -Weverything -ggdb -g -Wno-padded
CXX := clang++
CXXFLAGS := -Weverything -O0 -ggdb -g -std=c++11 -Wno-c++98-compat -Wno-padded -I../include -I.
LD := clang
LDFLAGS :=

AR := ar rcus

LIB := lib/simpledb.a
