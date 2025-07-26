BUILD_DIR ?= build
DEBUG_DIR ?= build-debug

.PHONY: all build build-debug clean test test-debug

all: clean build

build:
	@mkdir -p $(BUILD_DIR)
	cmake -S . -B $(BUILD_DIR) -DCMAKE_BUILD_TYPE=Release
	cmake --build $(BUILD_DIR)

build-debug:
	@mkdir -p $(DEBUG_DIR)
	cmake -S . -B $(DEBUG_DIR) -DCMAKE_BUILD_TYPE=Debug
	cmake --build $(DEBUG_DIR)

clean:
	@if [ -d $(BUILD_DIR) ]; then cmake --build $(BUILD_DIR) --target clean; fi
	@if [ -d $(DEBUG_DIR) ]; then cmake --build $(DEBUG_DIR) --target clean; fi
	@rm -rf $(BUILD_DIR) $(DEBUG_DIR)

test: clean build
	cd $(BUILD_DIR) && ctest -V

test-debug: build-debug
	cd $(DEBUG_DIR) && ctest -V -C Debug
