# Copyright 2025 Lorenzo Fritzsch
#
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
#
#    http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.

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
