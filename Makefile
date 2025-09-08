TARGET = TowerDefence
BUILD_DIR = build
CPP_FILES = $(shell find src -type f -name '*.cpp')
H_FILES = $(shell find src -type f -name '*.h')

.PHONY: all
all: build run

# ---------
# Build
# ---------
.PHONY: build
build:
	mkdir -p $(BUILD_DIR)
	cd $(BUILD_DIR) && cmake -DCMAKE_EXPORT_COMPILE_COMMANDS=ON .. && make

.PHONY: run
run: build
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

tidy:
	cd $(BUILD_DIR) && cmake -DCMAKE_EXPORT_COMPILE_COMMANDS=ON ..
	clang-tidy -p $(BUILD_DIR) $(CPP_FILES) $(H_FILES) --fix

# ---------
# Debugging
# ---------
.PHONY: build-debug
build-debug: $(BUILD_DIR)/Makefile
	cd $(BUILD_DIR) && cmake -DCMAKE_BUILD_TYPE=Debug -DCMAKE_EXPORT_COMPILE_COMMANDS=ON .. && $(MAKE)

.PHONY: run-debug
run-debug:
	cd $(BUILD_DIR) && gdb ./$(TARGET)

.PHONY: debug
debug: build-debug run-debug
