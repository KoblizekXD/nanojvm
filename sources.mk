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
BUILD_DIR = build
SRC_DIR = src
LIB_DIR = lib

# Toolchain configuration based on target
ifeq ($(TARGET),wasm)
    CC = emcc
    CXX = em++
    AS = nasm
else ifeq ($(TARGET),win32)
    CC = x86_64-w64-mingw32-gcc
    CXX = x86_64-w64-mingw32-g++
    AS = nasm
else
    # Linux/default
    CC = gcc
    CXX = g++
    AS = nasm
endif

SRC_C       := $(shell find $(SRC_DIR) -name '*.c')
SRC_CPP     := $(shell find $(SRC_DIR) -name '*.cpp')
SRC_ASM     := $(shell find $(SRC_DIR) -name '*.asm')

define TO_OBJ
$(addprefix $(BUILD_DIR)/src/, $(patsubst %.c,%.o,$(patsubst %.cpp,%.o,$(patsubst %.asm,%.o,$(1)))))
endef

OBJS := \
  $(call TO_OBJ,$(SRC_C)) \
  $(call TO_OBJ,$(SRC_CPP)) \
  $(call TO_OBJ,$(SRC_ASM)) \

INCLUDES := -I$(SRC_DIR) -I$(LIB_DIR)/miniz -I$(LIB_DIR)/classparse/src/

CFLAGS_C11 := -std=c11
CXXFLAGS_CXX11 := -std=c++11
WARNINGS := -Wall -Wextra -Wno-unused-parameter

# Dev flags
DEV_CFLAGS := $(CFLAGS_C11) $(WARNINGS) -pg -g -O0 -fsanitize=address,undefined \
              -fno-omit-frame-pointer -fstack-protector-strong -DDEBUG
DEV_CXXFLAGS := $(CXXFLAGS_CXX11) $(WARNINGS) -pg -g -O0 -fsanitize=address,undefined \
                -fno-omit-frame-pointer -fstack-protector-strong -DDEBUG
DEV_ASFLAGS := -f elf64 -g -F dwarf

# Prod flags
PROD_CFLAGS := $(CFLAGS_C11) $(WARNINGS) -O3 -flto -DNDEBUG -fomit-frame-pointer \
               -march=native -fno-stack-protector
PROD_CXXFLAGS := $(CXXFLAGS_CXX11) $(WARNINGS) -O3 -flto -DNDEBUG -fomit-frame-pointer \
                 -march=native -fno-stack-protector
PROD_ASFLAGS := -f elf64

dev: CFLAGS = $(DEV_CFLAGS)
dev: CXXFLAGS = $(DEV_CXXFLAGS)
dev: ASFLAGS = $(DEV_ASFLAGS)
dev: $(OBJS)

prod: CFLAGS = $(PROD_CFLAGS)
prod: CXXFLAGS = $(PROD_CXXFLAGS)
prod: ASFLAGS = $(PROD_ASFLAGS)
prod: $(OBJS)

$(BUILD_DIR)/src/%.o: %.c
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) $(INCLUDES) $(DEPFLAGS) -c $< -o $@

$(BUILD_DIR)/src/%.o: %.cpp
	@mkdir -p $(dir $@)
	$(CXX) $(CXXFLAGS) $(INCLUDES) $(DEPFLAGS) -c $< -o $@

$(BUILD_DIR)/src/%.o: %.asm
	@mkdir -p $(dir $@)
	$(AS) $(ASFLAGS) $< -o $@

-include $(OBJS:.o=.d)

$(BUILD_DIR):
	mkdir -p $@

clean:
	rm -rf $(BUILD_DIR)/src
