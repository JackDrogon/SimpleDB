-include define.mk

CXXFLAGS +=  -I./include

TEST_OBJ := test/db_test.o
TEST := test/db_test

.PHONY: all
.PHONY: clean
.PHONY: test

all: $(LIB)

$(LIB):
	$(MAKE) -C src

test: $(TEST)

$(TEST): $(LIB) $(TEST_OBJ)
	$(CXX) $(CXXFLAGS) -o $@ $^

clean:
	-$(MAKE) -C src clean
	-$(RM) -f $(TEST) $(TEST_OBJ)
