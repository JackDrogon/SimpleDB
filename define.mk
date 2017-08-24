CC := clang
CFLAGS := -Weverything -ggdb -g
CXX := clang++
CXXFLAGS := -Weverything -O0 -ggdb -g -std=c++11 -Wno-c++98-compat -I../include -I.
LD := clang
LDFLAGS :=

AR := ar rcus

LIB := lib/simpledb.a
