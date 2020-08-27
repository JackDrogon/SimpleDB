-include define.mk

CXXFLAGS +=  -I./include

DB_TEST := test/db_test
DB_TEST_OBJ := $(DB_TEST).o
DB_FULLLRU_TEST := test/db_full_lru_test
DB_FULLLRU_TEST_OBJ := $(DB_FULLLRU_TEST).o
TEST := $(DB_TEST) $(DB_FULLLRU_TEST)

.PHONY: all
.PHONY: clean
.PHONY: test
.PHONY: $(LIB)

all: $(LIB)

$(LIB):
	$(MAKE) -C src

test: $(TEST)

$(DB_TEST): $(LIB) $(DB_TEST_OBJ)
	$(CXX) $(CXXFLAGS) -o $@ $^

$(DB_FULLLRU_TEST): $(LIB) $(DB_FULLLRU_TEST_OBJ)
	$(CXX) $(CXXFLAGS) -o $@ $^

clean:
	-$(MAKE) -C src clean
	-$(RM) -f $(TEST) $(DB_TEST_OBJ) $(DB_FULLLRU_TEST) $(DB_FULLLRU_TEST_OBJ)
