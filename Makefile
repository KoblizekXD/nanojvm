# ===== Project Configuration =====
PROJECT_NAME := nanojvm
SRC_DIR := src
BUILD_DIR := build
BIN_DIR := bin
LIB_DIR := lib

# ===== Toolchain Configuration =====
CC := gcc
CXX := g++
ASM := nasm
LINKER := gcc

# ===== Build Mode Configuration =====
# Available modes: dev, prod
BUILD_MODE ?= dev

# ===== Source File Detection =====
C_SRCS := $(shell find $(SRC_DIR) -name '*.c')
C_SRCS += $(LIB_DIR)/miniz/miniz.c
CPP_SRCS := $(shell find $(SRC_DIR) -name '*.cpp')
ASM_SRCS := $(shell find $(SRC_DIR) -name '*.asm')

# ===== Object/Dependency Files =====
C_OBJS := $(patsubst $(SRC_DIR)/%,$(BUILD_DIR)/%,$(C_SRCS:.c=.o))
CPP_OBJS := $(patsubst $(SRC_DIR)/%,$(BUILD_DIR)/%,$(CPP_SRCS:.cpp=.o))
ASM_OBJS := $(patsubst $(SRC_DIR)/%,$(BUILD_DIR)/%,$(ASM_SRCS:.asm=.o))
OBJS := $(C_OBJS) $(CPP_OBJS) $(ASM_OBJS)
DEPS := $(OBJS:.o=.d)

# ===== Compiler Flags =====
COMMON_FLAGS := -Wall -Wextra -Wpedantic -Wno-unused-parameter -I$(SRC_DIR) -I$(LIB_DIR)/classparse/src/ -I$(LIB_DIR)/miniz/ -MMD -MP
COMMON_LD_FLAGS := -L $(LIB_DIR)/classparse/build/ -rdynamic -lclassparse -pthread

ifeq ($(BUILD_MODE),prod)
    OPT_FLAGS := -O3 -flto -DNDEBUG
    CFLAGS := $(COMMON_FLAGS) $(OPT_FLAGS) -std=gnu11
    CXXFLAGS := $(COMMON_FLAGS) $(OPT_FLAGS) -std=c++11
    ASMFLAGS := -f elf64
    LDFLAGS := $(OPT_FLAGS) $(COMMON_LD_FLAGS)
else
    OPT_FLAGS := -Og -g3 -DDEBUG -fsanitize=address,undefined -fno-omit-frame-pointer
    CFLAGS := $(COMMON_FLAGS) $(OPT_FLAGS) -std=gnu11
    CXXFLAGS := $(COMMON_FLAGS) $(OPT_FLAGS) -std=c++11
    ASMFLAGS := -f elf64 -g -F dwarf
    LDFLAGS := $(OPT_FLAGS) $(COMMON_LD_FLAGS)
endif

# ===== Targets Configuration =====
TARGET := $(BIN_DIR)/$(PROJECT_NAME)

TARGET_WIN32 := $(BIN_DIR)/$(PROJECT_NAME).exe
TARGET_WASM := $(BIN_DIR)/$(PROJECT_NAME).wasm

# ===== Build Rules =====
all: $(TARGET)

$(TARGET): $(OBJS) | $(BIN_DIR)
	$(LINKER) $^ -o $@ $(LDFLAGS)

$(BUILD_DIR)/%.o: $(SRC_DIR)/%.c | $(BUILD_DIR)
	@mkdir -p "$$(dirname $@)"
	$(CC) $(CFLAGS) -c $< -o $@

$(BUILD_DIR)/%.o: $(SRC_DIR)/%.cpp | $(BUILD_DIR)
	@mkdir -p "$$(dirname $@)"
	$(CXX) $(CXXFLAGS) -c $< -o $@

$(BUILD_DIR)/%.o: $(SRC_DIR)/%.asm | $(BUILD_DIR)
	@mkdir -p "$$(dirname $@)"
	$(ASM) $(ASMFLAGS) $< -o $@

$(BUILD_DIR) $(BIN_DIR):
	mkdir -p $@

-include $(DEPS)

# ===== Utility Targets =====
.PHONY: clean rebuild run debug

clean:
	rm -rf $(BUILD_DIR) $(BIN_DIR)

rebuild: clean all

run: all
	./$(TARGET)

debug: all
	gdb $(TARGET)

# ===== Additional Targets (Extensible) =====
win32: CC := x86_64-w64-mingw32-gcc
win32: CXX := x86_64-w64-mingw32-g++
win32: LINKER := x86_64-w64-mingw32-g++
win32: TARGET := $(TARGET_WIN32)
win32: all

wasm: CC := emcc
wasm: CXX := em++
wasm: LINKER := em++
wasm: TARGET := $(TARGET_WASM)
wasm: all
