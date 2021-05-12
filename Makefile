LIB_TARGET ?= libbittwiddling.so
SRC_DIR ?= src
INC_DIR ?= include
BENCH_DIR ?= benchmark
TEST_DIR ?= test
BUILD_DIR ?= build

SRCS := $(wildcard $(SRC_DIR)/*.c)
OBJS := $(patsubst $(SRC_DIR)/%.c,$(BUILD_DIR)/%.o,$(SRCS))
DEPS := $(OBJS:.o=.d)

BENCH_SRCS := $(wildcard $(BENCH_DIR)/*.c)
BENCH_OBJS := $(patsubst $(BENCH_DIR)/%.c,$(BUILD_DIR)/%.o,$(BENCH_SRCS))
BENCH_TARGET := $(BUILD_DIR)/bench
BENCH_DEPS := $(BENCH_OBJS:.o=.d)

TEST_SRCS := $(wildcard $(TEST_DIR)/*.c)
TEST_OBJS := $(patsubst $(TEST_DIR)/%.c,$(BUILD_DIR)/%.o,$(TEST_SRCS))
TEST_TARGET := $(BUILD_DIR)/tests
TEST_DEPS := $(TEST_OBJS:.o=.d)

INC_FLAGS := $(addprefix -I,$(INC_DIR))

CFLAGS ?= $(INC_FLAGS) -MMD -MP -fPIC -g -O2 -mavx -mavx2
LDFLAGS ?= -shared

all: build

build: create_build_dir $(BUILD_DIR)/$(LIB_TARGET)

create_build_dir:
	@mkdir -p build

$(OBJS): $(BUILD_DIR)/%.o : $(SRC_DIR)/%.c
	$(CC) $(CFLAGS) -c $< -o $@

$(BUILD_DIR)/$(LIB_TARGET): $(OBJS)
	$(CC) $(LDFLAGS) $(OBJS) -o $@ $(LOADLIBES) $(LDLIBS)

# BENCHMARK
benchmark: $(BENCH_TARGET)
	LD_LIBRARY_PATH=$(PWD)/$(BUILD_DIR) ./$(BENCH_TARGET)

$(BENCH_TARGET): build $(BENCH_OBJS)
	$(CC) $(INCFLAGS) $(BENCH_OBJS) -L$(BUILD_DIR) -lbittwiddling -o $@

$(BENCH_OBJS): $(BUILD_DIR)/%.o : $(BENCH_DIR)/%.c
	$(CC) $(CFLAGS) -c $< -o $@

# TEST
test: $(TEST_TARGET)
	LD_LIBRARY_PATH=$(PWD)/$(BUILD_DIR) ./$(TEST_TARGET)

$(TEST_TARGET): build $(TEST_OBJS)
	$(CC) $(INCFLAGS) $(TEST_OBJS) -L$(BUILD_DIR) -lbittwiddling -o $@

$(TEST_OBJS): $(BUILD_DIR)/%.o : $(TEST_DIR)/%.c
	$(CC) $(CFLAGS) -c $< -o $@

.PHONY: clean
clean:
	$(RM) $(BUILD_DIR)/$(LIB_TARGET) $(OBJS) $(DEPS) $(TEST_DEPS) $(BENCH_DEPS) $(BENCH_TARGET) $(BENCH_OBJS) $(TEST_OBJS) $(TEST_TARGET)

-include $(DEPS)
-include $(BENCH_DEPS)
-include $(TEST_DEPS)
