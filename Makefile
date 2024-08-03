.PHONY: all
all: format test build

.PHONY: format
format:
	@echo "Finding and formatting files..."
	find $(PWD)/1_Software -type f \( -name '*.cpp' -o -name '*.h' -o -name '*.c' -o -name '*.hpp' \) -print0 | xargs -0 clang-format -i
	@echo "Formatting complete."

.PHONY: build
build:
ifndef pf
	$(error "pf is not defined. Use 'make build pf=macos' or 'make build pf=linux'")
endif
	mkdir -p build
	cd build && \
	cmake -DBUILD_PLATFORM=$(pf) .. && \
	make

.PHONY: debug
debug:
ifndef pf
	$(error "pf is not defined. Use 'make debug pf=macos' or 'make debug pf=linux'")
endif
	mkdir -p build
	cd build && \
	cmake -DBUILD_PLATFORM=$(pf) -DCMAKE_BUILD_TYPE=Debug .. && \
	make

.PHONY: clean
clean:
	rm -rf build
