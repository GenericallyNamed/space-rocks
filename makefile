# Directories
SRC_DIR = src
BUILD_DIR = build


# Source Files
HTML_FILES = $(wildcard $(SRC_DIR)/*.html)
SRC_JS_FILES = $(wildcard $(SRC_DIR)/js/*.js)
CSS_FILES = $(wildcard $(SRC_DIR)/css/*.css)
CPP_FILES = $(wildcard $(SRC_DIR)/cpp/*.cpp)
HPP_FILES = $(wildcard $(SRC_DIR)/cpp/*.hpp)
MAIN_CPP_FILE = $(SRC_DIR)/cpp/main.cpp

SRC_TAILWIND_FILE = $(SRC_DIR)/input.css
SRC_INDEX_HTML_FILE = $(SRC_DIR)/index.html
SRC_FAVICON_FILE = $(SRC_DIR)/favicon.ico

# Build files

BUILD_TAILWIND_FILE = $(BUILD_DIR)/css/tailwind.css
BUILD_FAVICON_FILE = $(BUILD_DIR)/favicon.ico



# clean:
# 	@echo "Cleaning build directory"
# 	@rm -rf $(BUILD_DIR)

release: index.html css js tailwind wasm
	@echo "Release build complete"


# build:
# 	@echo "Creating build directory"
# 	# Removing old build if applicable
# 	@rm -rf $(BUILD_DIR)
# 	@mkdir -p $(BUILD_DIR)

# File Targets
index.html: $(SRC_INDEX_HTML_FILE)
	@echo "Copying index.html to build directory"
	@cp $(SRC_INDEX_HTML_FILE) $(BUILD_DIR)/index.html

# CSS Files
css: $(CSS_FILES)
	@echo "Copying CSS files to build directory"
	@mkdir -p $(BUILD_DIR)/css
	@cp $(CSS_FILES) $(BUILD_DIR)/css

tailwind: $(SRC_TAILWIND_FILE)
	@echo "Compiling Tailwind CSS"
	@npx tailwindcss-cli@latest -i $(SRC_TAILWIND_FILE) -o $(BUILD_TAILWIND_FILE)
$(BUILD_DIR)/js/main.js: $(SRC_DIR)/js/main.js
	cp $(SRC_DIR)/js/main.js $(BUILD_DIR)/js

.PHONY: js
js: $(BUILD_DIR)/js/main.js
	echo "Copying JS files to build directory"
	mkdir -p $(BUILD_DIR)/js

favicon: $(SRC_FAVICON_FILE)
	@echo "Copying favicon to build directory"
	@cp $(SRC_FAVICON_FILE) $(BUILD_FAVICON_FILE)

# WASM Related items
EMCC_EXPORTS = -sEXPORTED_FUNCTIONS=_init,_key_pressed,_key_released,_get_frame

$(BUILD_DIR)/js/asteroids.js: $(CPP_FILES) $(HPP_FILES)
	emcc $(EMCC_EXPORTS) --bind $(CPP_FILES) -o $@


.PHONY: wasm
wasm: $(BUILD_DIR)/js/asteroids.js

