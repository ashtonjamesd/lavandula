SRCS_LAVANDULA = $(filter-out lavandula/main.c, $(shell find lavandula -name "*.c"))

SRCS = app/app.c app/routes.c $(wildcard app/controllers/*.c) $(wildcard app/middleware/*.c)
CFLAGS = -Wall -Wextra -lsqlite3 -Isrc -Ilavandula/include

all:
	mkdir -p build
	gcc $(SRCS) $(SRCS_LAVANDULA) $(CFLAGS) -o build/a
