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

build/warehouse:
	${CC} ${CC_FLAGS} -o build/warehouse.o src/warehouse.c

build/json:
	${CC} ${CC_FLAGS} -o build/json.o src/cJSON.c

test: settings build/test build/warehouse build/json
	${LD} build/test.o build/warehouse.o build/json.o -lm -o test
	./test

build: settings build/warehouse build/json
	${LD} build/warehouse.o build/json.o -lm -o warehouse
