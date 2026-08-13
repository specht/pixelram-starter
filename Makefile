EMCC := emcc

PROGRAM := main
ARGS :=

PIXELRAM_VERSION := v0.1.19
PIXELRAM_BASE := https://raw.githubusercontent.com/specht/pixelram/$(PIXELRAM_VERSION)
PIXELRAM_ROOT := .pixelram
PIXELRAM_DIR := $(PIXELRAM_ROOT)/$(PIXELRAM_VERSION)

CFLAGS := -O2 -std=c11 -Wall -Wextra -Wpedantic -I$(PIXELRAM_DIR)

BUILD_DIR := .build
ARGS_JS := $(BUILD_DIR)/pixelram_args.js

WEB_FLAGS := \
	-DPLATFORM_WEB \
	-sUSE_GLFW=3 \
	-sASYNCIFY \
	-sFORCE_FILESYSTEM \
	-sSINGLE_FILE=1 \
	-sSINGLE_FILE_BINARY_ENCODE=0 \
	--pre-js $(ARGS_JS) \
	--shell-file $(PIXELRAM_DIR)/shell.html

PIXELRAM_FILES := \
	$(PIXELRAM_DIR)/pixelram.c \
	$(PIXELRAM_DIR)/pixelram.h \
	$(PIXELRAM_DIR)/shell.html \
	$(PIXELRAM_DIR)/make_args.py

.PHONY: all clean distclean FORCE

all: $(PROGRAM).html

$(PIXELRAM_DIR):
	mkdir -p $@

$(BUILD_DIR):
	mkdir -p $@

$(PIXELRAM_DIR)/pixelram.c: | $(PIXELRAM_DIR)
	@echo "Downloading PixelRAM $(PIXELRAM_VERSION)..."
	@rm -f $@.tmp
	@curl -fsSL "$(PIXELRAM_BASE)/pixelram.c" -o $@.tmp
	@mv $@.tmp $@

$(PIXELRAM_DIR)/pixelram.h: | $(PIXELRAM_DIR)
	@rm -f $@.tmp
	@curl -fsSL "$(PIXELRAM_BASE)/pixelram.h" -o $@.tmp
	@mv $@.tmp $@

$(PIXELRAM_DIR)/shell.html: | $(PIXELRAM_DIR)
	@rm -f $@.tmp
	@curl -fsSL "$(PIXELRAM_BASE)/shell.html" -o $@.tmp
	@mv $@.tmp $@

$(PIXELRAM_DIR)/make_args.py: | $(PIXELRAM_DIR)
	@rm -f $@.tmp
	@curl -fsSL "$(PIXELRAM_BASE)/tools/make_args.py" -o $@.tmp
	@mv $@.tmp $@

FORCE:

$(ARGS_JS): FORCE $(PIXELRAM_DIR)/make_args.py | $(BUILD_DIR)
	@python3 $(PIXELRAM_DIR)/make_args.py $@.tmp $(ARGS)
	@if ! cmp -s $@.tmp $@ 2>/dev/null; then \
		mv $@.tmp $@; \
	else \
		rm -f $@.tmp; \
	fi

$(PROGRAM).html: $(PROGRAM).c $(PIXELRAM_FILES) $(ARGS_JS)
	$(EMCC) $(CFLAGS) $(WEB_FLAGS) \
		$(PROGRAM).c $(PIXELRAM_DIR)/pixelram.c \
		-lraylib \
		-o $@

clean:
	rm -rf $(BUILD_DIR) $(PROGRAM).html

distclean: clean
	rm -rf $(PIXELRAM_ROOT)
