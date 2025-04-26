CC := gcc
LD := gcc
FREESTANDING ?= 0
DISABLE_MINIZ ?= 0
DISABLE_CLASSPARSE ?= 0

BUILD_ROOT := ../build
MINIZ_SRC := miniz/miniz.c
MINIZ_OBJ := $(BUILD_ROOT)/miniz/miniz.o
CLASSPARSE_BUILD_DIR := classparse/build
CLASSPARSE_OBJ_DEST := $(BUILD_ROOT)/classparse

DEV_FLAGS := -Wall -Wextra -O0 -g -pg
PROD_FLAGS := -Wall -Wextra -O3

all: dev

MODE ?= dev

dev: CFLAGS := $(DEV_FLAGS)
dev: | $(BUILD_ROOT) $(CLASSPARSE_OBJ_DEST) $(BUILD_ROOT)/miniz
	$(MAKE) build MODE=dev

prod: CFLAGS := $(PROD_FLAGS)
prod: | $(BUILD_ROOT) $(CLASSPARSE_OBJ_DEST) $(BUILD_ROOT)/miniz
	$(MAKE) build MODE=prod

build: miniz_build classparse_build

miniz_build:
ifeq ($(DISABLE_MINIZ),0)
	$(CC) $(CFLAGS) -c $(MINIZ_SRC) -o $(MINIZ_OBJ)
	@echo "[MINIZ] Built $(MINIZ_OBJ)"
else
	@echo "[MINIZ] Build disabled (DISABLE_MINIZ=1)"
endif

classparse_build:
ifeq ($(DISABLE_CLASSPARSE),0)
	@echo "[CLASSPARSE] Building with FREESTANDING=$(FREESTANDING)"
	$(MAKE) -C classparse FREESTANDING=$(FREESTANDING) $(MODE)
	@mkdir -p $(CLASSPARSE_OBJ_DEST)
	@cp $(CLASSPARSE_BUILD_DIR)/*.o $(CLASSPARSE_OBJ_DEST)/
	@echo "[CLASSPARSE] Objects copied to $(CLASSPARSE_OBJ_DEST)"
else
	@echo "[CLASSPARSE] Build disabled (DISABLE_CLASSPARSE=1)"
endif

$(BUILD_ROOT):
	@mkdir -p $@

$(CLASSPARSE_OBJ_DEST):
	@mkdir -p $@

$(BUILD_ROOT)/miniz:
	@mkdir -p $@

clean:
	rm -rf $(BUILD_ROOT)/classparse/ $(BUILD_ROOT)/miniz/
	@if [ "$(DISABLE_CLASSPARSE)" = "0" ]; then \
		$(MAKE) -C classparse clean; \
	fi

.PHONY: all dev prod build miniz_build classparse_build clean
