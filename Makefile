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

build/test: src/test.c
	${CC} ${CC_FLAGS} -o build/test.o src/test.c

build/structs: src/structs.c
	${CC} ${CC_FLAGS} -o build/structs.o src/structs.c

build/json: src/cJSON.c
	${CC} ${CC_FLAGS} -o build/json.o src/cJSON.c

build/webserver: src/webserver.c
	${CC} ${CC_FLAGS} -o build/webserver.o src/webserver.c

build/warehouse: src/warehouse.c
	${CC} ${CC_FLAGS} -o build/warehouse.o src/warehouse.c

build/producer: src/producer.c
	${CC} ${CC_FLAGS} -o build/producer.o src/producer.c

test: settings build/structs build/test build/json
	${LD} build/structs.o build/test.o build/json.o -lm -o test
	./test

build/ware: settings build/structs build/webserver build/warehouse build/json
	${LD} build/structs.o build/webserver.o -pthread build/warehouse.o build/json.o -lm -o warehouse

build/prod: settings build/structs build/webserver build/producer build/json
	${LD} build/structs.o build/webserver.o -pthread build/producer.o build/json.o -lm -o producer

build: build/ware build/prod