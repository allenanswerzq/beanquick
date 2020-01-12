
style:
	find beanquick -name "*.cc" -o -name "*.h" | xargs -t -I{} clang-format -i {}

SRC_DIR=./beanquick
DST_DIR=./beanquick

build:
	protoc -I=$(SRC_DIR) --cpp_out=$(DST_DIR) $(SRC_DIR)/beanquick.proto


