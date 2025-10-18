SRCS = $(shell find src -name "*.c")

TEST_SRCS = $(wildcard test/*.c)
CC = gcc
COMMON_FLAGS = -Wall -Wextra -Werror -fstack-protector-strong -Wstrict-overflow -Wformat-security -Wno-unused-parameter -lsqlite3 -Isrc
CFLAGS = $(COMMON_FLAGS) -D_FORTIFY_SOURCE=2
TEST_CFLAGS = $(COMMON_FLAGS) -g3 -O0 -fsanitize=address -fsanitize=undefined -fno-omit-frame-pointer

all:
	mkdir -p build
	$(CC) $(SRCS) $(CFLAGS) -o build/lavu

test:
	mkdir -p build
	$(CC) $(filter-out src/main.c, $(SRCS)) $(TEST_SRCS) $(TEST_CFLAGS) -o build/test_runner
	./build/test_runner

install:
	bash install.sh

clean:
	rm -rf build

.PHONY: all test install clean
