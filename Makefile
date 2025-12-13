TARGET = TowerDefence
BUILD_DIR = build
CPP_FILES = $(shell find src -type f -name '*.cpp')
H_FILES = $(shell find src -type f -name '*.h')

CMAKE_FLAGS = -DCMAKE_EXPORT_COMPILE_COMMANDS=ON
DEBUG_FLAGS = -DCMAKE_BUILD_TYPE=Debug \
              -DCMAKE_CXX_FLAGS="-fsanitize=address -fno-omit-frame-pointer -O1"
RELEASE_FLAGS = -DCMAKE_BUILD_TYPE=Release \
                -DCMAKE_CXX_FLAGS="-O3 -march=native -DNDEBUG"
PROFILE_FLAGS = -DCMAKE_BUILD_TYPE=Release \
                -DCMAKE_CXX_FLAGS="-O2 -pg" \
                -DCMAKE_EXE_LINKER_FLAGS="-pg"

.PHONY: all
all: build run

# ---------
# Build
# ---------
.PHONY: build
build:
	mkdir -p $(BUILD_DIR)
	cd $(BUILD_DIR) && cmake $(CMAKE_FLAGS) $(RELEASE_FLAGS) .. && make

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
	cd $(BUILD_DIR) && cmake $(CMAKE_FLAGS) ..
	clang-tidy -p $(BUILD_DIR) $(CPP_FILES) $(H_FILES) --fix

# ---------
# Debugging
# ---------
.PHONY: build-debug
build-debug:
	mkdir -p $(BUILD_DIR)
	cd $(BUILD_DIR) && cmake $(CMAKE_FLAGS) $(DEBUG_FLAGS) .. && $(MAKE)

run-debug: build-debug
	cd $(BUILD_DIR) && \
	LSAN_OPTIONS=suppressions=../lsan.supp \
	ASAN_OPTIONS=detect_leaks=1:abort_on_error=0 \
	./TowerDefence 2> >(tee asan.log >&2)

.PHONY: debug
debug: build-debug run-debug

# ---------
# Profiling
# ---------
.PHONY: build-profile
build-profile:
	mkdir -p $(BUILD_DIR)
	cd $(BUILD_DIR) && cmake $(CMAKE_FLAGS) $(PROFILE_FLAGS) .. && $(MAKE)

.PHONY: profile
profile: build-profile
	cd $(BUILD_DIR) && \
	./$(TARGET) && \
	gprof ./$(TARGET) gmon.out > profile.txt
