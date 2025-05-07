UNAME_S := $(shell uname -s)
ifeq ($(UNAME_S),Linux)
    DEFAULT_TARGET = linux
else ifeq ($(UNAME_S),Windows_NT)
    DEFAULT_TARGET = win32
else
    DEFAULT_TARGET = linux
endif

TARGET ?= $(DEFAULT_TARGET)
BUILD_DIR = build
BIN_DIR = bin
SRC_DIR = src
LIB_DIR = lib

BUILD_AS_LIB ?= 0

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
ifeq ($(BUILD_AS_LIB),1)
    EXT = .so
else
	EXT =
endif
endif

SRC_C       := $(shell find $(SRC_DIR) -name '*.c')
SRC_CPP     := $(shell find $(SRC_DIR) -name '*.cpp')
SRC_ASM     := $(shell find $(SRC_DIR) -name '*.asm')

define TO_OBJ
$(addprefix $(BUILD_DIR)/, $(patsubst %.c,%.o,$(patsubst %.cpp,%.o,$(patsubst %.asm,%.o,$(1)))))
endef

INCLUDES := -I$(SRC_DIR) -I$(LIB_DIR)/miniz -I$(LIB_DIR)/classparse/src/

LINK_CLASSPARSE ?= 0
LINK_MINIZ ?= 0

CFLAGS_C11 := -std=c11
CXXFLAGS_CXX11 := -std=c++11
WARNINGS := -Wall -Wextra -Wno-unused-parameter
FREESTANDING ?= 0

ifeq ($(FREESTANDING),1)
	LINK_MINIZ = 0
	LINK_CLASSPARSE = 0
endif

EXEC = $(BIN_DIR)/nanojvm$(EXT)

COMMON_LDFLAGS := -rdynamic

# Dev flags
DEV_CFLAGS := $(CFLAGS_C11) $(WARNINGS) -pg -g -O0 -fsanitize=address,undefined \
              -fno-omit-frame-pointer -fstack-protector-strong -DDEBUG
DEV_CXXFLAGS := $(CXXFLAGS_CXX11) $(WARNINGS) -pg -g -O0 -fsanitize=address,undefined \
                -fno-omit-frame-pointer -fstack-protector-strong -DDEBUG
DEV_ASFLAGS := -f elf64 -g -F dwarf
DEV_LDFLAGS := -fsanitize=address,undefined -pg $(COMMON_LDFLAGS)

# Prod flags
PROD_CFLAGS := $(CFLAGS_C11) $(WARNINGS) -O3 -flto -DNDEBUG -fomit-frame-pointer \
               -march=native -fno-stack-protector
PROD_CXXFLAGS := $(CXXFLAGS_CXX11) $(WARNINGS) -O3 -flto -DNDEBUG -fomit-frame-pointer \
                 -march=native -fno-stack-protector
PROD_ASFLAGS := -f elf64
PROD_LDFLAGS := -flto -s $(COMMON_LDFLAGS)

ifneq ($(FREESTANDING),1)
    ifeq ($(LINK_CLASSPARSE),1)
        DEV_LDFLAGS += -lclassparse -L$(LIB_DIR)/classparse/bin
        PROD_LDFLAGS += -lclassparse -L$(LIB_DIR)/classparse/bin
    endif
    
    ifeq ($(LINK_MINIZ),1)
        DEV_LDFLAGS += -lminiz
        PROD_LDFLAGS += -lminiz
    endif
endif

.PHONY: all dev prod dev-all prod-all clean

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

ifeq ($(BUILD_AS_LIB),1)
	LDFLAGS += -shared
endif

dev-all:
	$(MAKE) -C $(LIB_DIR) dev TARGET=$(TARGET)
	$(MAKE) dev

prod-all:
	$(MAKE) -C $(LIB_DIR) prod TARGET=$(TARGET)
	$(MAKE) prod

$(EXEC): $(OBJS) | $(BIN_DIR)
	$(LD) $^ -o $@ $(LDFLAGS)

$(BUILD_DIR)/%.o: %.cpp
	@mkdir -p $(dir $@)
	$(CXX) $(CXXFLAGS) $(INCLUDES) $(DEPFLAGS) -c $< -o $@

$(BUILD_DIR)/%.o: %.asm
	@mkdir -p $(dir $@)
	$(AS) $(ASFLAGS) $< -o $@

$(BIN_DIR):
	mkdir -p $@

$(BUILD_DIR):
	mkdir -p $@

-include $(OBJS:.o=.d)

clean:
	rm -rf $(BUILD_DIR) $(BIN_DIR)


