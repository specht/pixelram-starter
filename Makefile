EMCC := emcc

PROGRAM := main
ARGS :=

PIXELRAM_VERSION := v0.1.22
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

.PHONY: all clean distclean crystal FORCE

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

# Crystal Caves (1991, shareware episode) running on PixelRAM: the full
# PixelRAM repository at the same version builds it, the result lands in
# spiele/. Takes a minute or two the first time.
PIXELRAM_REPO := $(PIXELRAM_ROOT)/$(PIXELRAM_VERSION)-repo
GAMES_DIR := spiele

crystal:
	@if [ ! -d "$(PIXELRAM_REPO)/.git" ]; then \
		echo "Downloading PixelRAM $(PIXELRAM_VERSION) with its game ports..."; \
		git clone -q --depth 1 --branch $(PIXELRAM_VERSION) https://github.com/specht/pixelram.git "$(PIXELRAM_REPO)"; \
	fi
	$(MAKE) -C "$(PIXELRAM_REPO)" crystal
	mkdir -p $(GAMES_DIR)
	cp $(PIXELRAM_REPO)/build/crystal.* $(GAMES_DIR)/
	@echo
	@echo "Ready: $(GAMES_DIR)/crystal.html"

clean:
	rm -rf $(BUILD_DIR) $(PROGRAM).html

distclean: clean
	rm -rf $(PIXELRAM_ROOT) $(GAMES_DIR)
