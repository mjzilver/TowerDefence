TARGET = SynergyTowers
BUILD_DIR = build
CPP_FILES = $(shell find src -type f -name '*.cpp')

.PHONY: all
all: build run

.PHONY: install-deps
install-deps:
	sudo apt-get update 
	sudo apt-get install libglew-dev libglfw3-dev libglm-dev libopengl-dev libfreetype6 libfreetype6-dev fonts-dejavu-core

.PHONY: install-tools
install-tools:
	sudo apt-get install -y clang-format clang-tidy gdb valgrind

.PHONY: build
build:
	mkdir -p $(BUILD_DIR)
	cd $(BUILD_DIR) && cmake .. && make

.PHONY: run
run:
	./$(BUILD_DIR)/$(TARGET)

.PHONY: clean
clean:
	rm -rf $(BUILD_DIR)

# ---------
# Formatting
# ---------

.PHONY: format
format:
	clang-format -i $(CPP_FILES)

# ---------
# Debugging
# ---------

.PHONY: build-debug
build-debug: $(BUILD_DIR)/Makefile
	cd $(BUILD_DIR) && cmake -DCMAKE_BUILD_TYPE=Debug .. && $(MAKE)

.PHONY: run-debug
run-debug:
	cd $(BUILD_DIR) && gdb ./$(TARGET)

.PHONY: debug
debug: build-debug run-debug
