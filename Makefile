# Root Makefile

# Determine default target platform
UNAME_S := $(shell uname -s)
ifeq ($(UNAME_S),Linux)
    DEFAULT_TARGET = linux
else ifeq ($(UNAME_S),Windows_NT)
    DEFAULT_TARGET = win32
else
    DEFAULT_TARGET = linux
endif

# Configuration
TARGET ?= $(DEFAULT_TARGET)
FREESTANDING ?= 0
BUILD_DIR = build
BIN_DIR = bin
SRC_DIR = src
LIB_DIR = lib

# Toolchain configuration based on target
ifeq ($(TARGET),wasm)
    CC = emcc
    CXX = em++
    AS = nasm
    LD = emcc
    EXT = .wasm
else ifeq ($(TARGET),win32)
    CC = x86_64-w64-mingw32-gcc
    CXX = x86_64-w64-mingw32-g++
    AS = nasm
    LD = x86_64-w64-mingw32-g++
    EXT = .exe
else
    # Linux/default
    CC = gcc
    CXX = g++
    AS = nasm
    LD = g++
    EXT =
endif

SRC_C       := $(shell find $(SRC_DIR) -name '*.c')
SRC_CPP     := $(shell find $(SRC_DIR) -name '*.cpp')
SRC_ASM     := $(shell find $(SRC_DIR) -name '*.asm')

define TO_OBJ
$(addprefix $(BUILD_DIR)/, $(patsubst %.c,%.o,$(patsubst %.cpp,%.o,$(patsubst %.asm,%.o,$(1)))))
endef

OBJS := \
  $(call TO_OBJ,$(SRC_C)) \
  $(call TO_OBJ,$(SRC_CPP)) \
  $(call TO_OBJ,$(SRC_ASM)) \

ifndef DISABLE_MINIZ
	MINIZ_C     := $(wildcard $(LIB_DIR)/miniz/*.c)
	MINIZ_CPP   := $(wildcard $(LIB_DIR)/miniz/*.cpp)
	OBJS += $(call TO_OBJ,$(MINIZ_C)) $(call TO_OBJ,$(MINIZ_CPP))
endif

# Include directories
INCLUDES := -I$(SRC_DIR) -I$(LIB_DIR)/miniz -I$(LIB_DIR)/classparse/src/

# Common flags
CFLAGS_C11 := -std=c11
CXXFLAGS_CXX11 := -std=c++11
WARNINGS := -Wall -Wextra -Wno-unused-parameter

# Dev flags
DEV_CFLAGS := $(CFLAGS_C11) $(WARNINGS) -pg -g -O0 -fsanitize=address,undefined \
              -fno-omit-frame-pointer -fstack-protector-strong -DDEBUG
DEV_CXXFLAGS := $(CXXFLAGS_CXX11) $(WARNINGS) -pg -g -O0 -fsanitize=address,undefined \
                -fno-omit-frame-pointer -fstack-protector-strong -DDEBUG
DEV_ASFLAGS := -f elf64 -g -F dwarf
DEV_LDFLAGS := -fsanitize=address,undefined -pg -rdynamic -pthread -lm -lclassparse \
               -L$(LIB_DIR)/classparse/bin

# Prod flags
PROD_CFLAGS := $(CFLAGS_C11) $(WARNINGS) -O3 -flto -DNDEBUG -fomit-frame-pointer \
               -march=native -fno-stack-protector
PROD_CXXFLAGS := $(CXXFLAGS_CXX11) $(WARNINGS) -O3 -flto -DNDEBUG -fomit-frame-pointer \
                 -march=native -fno-stack-protector
PROD_ASFLAGS := -f elf64
PROD_LDFLAGS := -flto -s -pthread -lm -lclassparse -L$(LIB_DIR)/classparse/bin

ifdef DISABLE_MINIZ
	PROD_CFLAGS += -DDISABLE_MINIZ
	PROD_CXXFLAGS += -DDISABLE_MINIZ
	DEV_CFLAGS += -DDISABLE_MINIZ
	DEV_CXXFLAGS += -DDISABLE_MINIZ
endif

# Dependency generation
DEPFLAGS = -MT $@ -MMD -MP -MF $(BUILD_DIR)/$*.d

# Final executable name
EXEC = $(BIN_DIR)/nanojvm$(EXT)

# Phony targets
.PHONY: all dev prod dev-all prod-all clean clean-all

# Dev build
dev: CFLAGS = $(DEV_CFLAGS)
dev: CXXFLAGS = $(DEV_CXXFLAGS)
dev: ASFLAGS = $(DEV_ASFLAGS)
dev: LDFLAGS = $(DEV_LDFLAGS)
dev: $(EXEC)

all: prod-all

# Prod build
prod: CFLAGS = $(PROD_CFLAGS)
prod: CXXFLAGS = $(PROD_CXXFLAGS)
prod: ASFLAGS = $(PROD_ASFLAGS)
prod: LDFLAGS = $(PROD_LDFLAGS)
prod: $(EXEC)

dev-all:
	$(MAKE) -C $(LIB_DIR)/classparse dev TARGET=$(TARGET)
ifeq($(FREESTANDING), 1)
	sudo $(MAKE) -C $(LIB_DIR)/classparse install
endif
	$(MAKE) -C std
	$(MAKE) dev

prod-all:
	$(MAKE) -C $(LIB_DIR)/classparse prod TARGET=$(TARGET)
	sudo $(MAKE) -C $(LIB_DIR)/classparse install
	$(MAKE) -C std
	$(MAKE) prod

# Link rule
$(EXEC): $(OBJS) | $(BIN_DIR)
	$(LD) $^ -o $@ $(LDFLAGS)

# Compilation rules
$(BUILD_DIR)/%.o: %.c
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) $(INCLUDES) $(DEPFLAGS) -c $< -o $@

# Generic C++ file rule
$(BUILD_DIR)/%.o: %.cpp
	@mkdir -p $(dir $@)
	$(CXX) $(CXXFLAGS) $(INCLUDES) $(DEPFLAGS) -c $< -o $@

# Generic ASM file rule
$(BUILD_DIR)/%.o: %.asm
	@mkdir -p $(dir $@)
	$(AS) $(ASFLAGS) $< -o $@

$(BUILD_DIR)/miniz/%.o: $(LIB_DIR)/miniz/%.c | $(BUILD_DIR)/miniz
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) $(INCLUDES) $(DEPFLAGS) -c $< -o $@

$(BUILD_DIR)/miniz/%.o: $(LIB_DIR)/miniz/%.cpp | $(BUILD_DIR)/miniz
	@mkdir -p $(dir $@)
	$(CXX) $(CXXFLAGS) $(INCLUDES) $(DEPFLAGS) -c $< -o $@

# Create directories
$(BIN_DIR):
	mkdir -p $@

$(BUILD_DIR):
	mkdir -p $@
	mkdir -p $(BUILD_DIR)/miniz

$(BUILD_DIR)/miniz:
	mkdir -p $@

# Clean
clean:
	rm -rf $(BUILD_DIR) $(BIN_DIR)

clean-all: clean
	$(MAKE) -C $(LIB_DIR)/classparse clean
	$(MAKE) -C std clean

# Include dependencies
-include $(OBJS:.o=.d)

# Print help
help:
	@echo "Available targets:"
	@echo "  dev        - Build development version (debug symbols, sanitizers)"
	@echo "  prod       - Build production version (optimized, no debug)"
	@echo "  dev-all    - Build all projects in development mode"
	@echo "  prod-all   - Build all projects in production mode"
	@echo "  clean      - Clean build and bin directories"
	@echo "  clean-all  - Clean all projects"
	@echo ""
	@echo "Variables:"
	@echo "  TARGET     - Target platform (wasm, linux, win32), default: $(DEFAULT_TARGET)"
