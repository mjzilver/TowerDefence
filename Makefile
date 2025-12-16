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
MAKE_FLAGS = -j4

.PHONY: all
all: build run

# ---------
# Build
# ---------
.PHONY: build
build:
	mkdir -p $(BUILD_DIR)
	cd $(BUILD_DIR) && cmake $(CMAKE_FLAGS) $(RELEASE_FLAGS) .. && $(MAKE) $(MAKE_FLAGS)

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
	clang-format -i $(CPP_FILES) $(H_FILES)

tidy:
	cd $(BUILD_DIR) && cmake $(CMAKE_FLAGS) ..
	clang-tidy -p $(BUILD_DIR) $(CPP_FILES) $(H_FILES) --fix

# ---------
# Debugging
# ---------
.PHONY: build-debug
build-debug:
	mkdir -p $(BUILD_DIR)
	cd $(BUILD_DIR) && cmake $(CMAKE_FLAGS) $(DEBUG_FLAGS) .. && $(MAKE) $(MAKE_FLAGS)

run-debug: build-debug
	cd $(BUILD_DIR) && \
	LSAN_OPTIONS=suppressions=../lsan.supp \
	ASAN_OPTIONS=\
	detect_leaks=1:\
	abort_on_error=0:\
	symbolize=1:\
	fast_unwind_on_malloc=0:\
	malloc_context_size=50 \
	LSAN_OPTIONS=verbosity=1 \
	./$(TARGET) 2>&1 | tee ./asan.log

.PHONY: debug
debug: build-debug run-debug

# ---------
# Profiling
# ---------
.PHONY: build-profile
build-profile:
	mkdir -p $(BUILD_DIR)
	cd $(BUILD_DIR) && cmake $(CMAKE_FLAGS) $(PROFILE_FLAGS) .. && $(MAKE) $(MAKE_FLAGS)

.PHONY: profile
profile: build-profile
	cd $(BUILD_DIR) && \
	./$(TARGET) && \
	gprof ./$(TARGET) gmon.out > profile.txt