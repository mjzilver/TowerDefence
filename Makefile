TARGET = SynergyTowers
BUILD_DIR = build

.PHONY: all
all: build run

.PHONY: install-deps
install-deps:
	sudo apt-get update
	sudo apt-get install libglew-dev libglfw3-dev libglm-dev libopengl-dev

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