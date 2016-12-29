CC = gcc
# CC_FLAGS = -Wall -c -m32 -ffreestanding -I src/kernel/include -nostdinc -ggdb
CC_FLAGS = -Wall -std=gnu11 -c  -ffreestanding -I src/kernel/include -ggdb
# CC_FLAGS = -Wall -c -m32 -ffreestanding  -ggdb

LD = gcc
# LD = ld

### Clean ###

.PHONY: clean
clean:
	rm build/*

settings:
	mkdir -p build

build/test:
	${CC} ${CC_FLAGS} -o build/test.o src/test.c

build/structs:
	${CC} ${CC_FLAGS} -o build/structs.o src/structs.c

build/json:
	${CC} ${CC_FLAGS} -o build/json.o src/cJSON.c
build/webserver:
	${CC} ${CC_FLAGS} -o build/webserver.o src/webserver.c

build/warehouse:
	${CC} ${CC_FLAGS} -o build/warehouse.o src/warehouse.c


test: settings build/structs build/test build/json
	${LD} build/structs.o build/test.o build/json.o -lm -o test
	./test

build: settings build/structs build/webserver build/warehouse build/json
	${LD} build/structs.o build/webserver.o -pthread build/warehouse.o build/json.o -lm -o warehouse
