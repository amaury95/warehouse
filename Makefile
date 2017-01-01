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

build/tokenizer: src/tokenizer.c
	${CC} ${CC_FLAGS} -o build/tokenizer.o src/tokenizer.c


build/warehouse: src/warehouse.c
	${CC} ${CC_FLAGS} -o build/warehouse.o src/warehouse.c

build/producer: src/producer.c
	${CC} ${CC_FLAGS} -o build/producer.o src/producer.c

build/consumer: src/consumer.c
	${CC} ${CC_FLAGS} -o build/consumer.o src/consumer.c

test: settings build/structs build/test build/json
	${LD} build/structs.o build/test.o build/json.o -lm -o test
	./test

build/ware: settings build/tokenizer build/structs build/webserver build/warehouse build/json
	${LD} build/tokenizer.o build/structs.o build/webserver.o -pthread build/warehouse.o build/json.o -lm -o warehouse

build/prod: settings build/tokenizer build/structs build/webserver build/producer build/json
	${LD} build/tokenizer.o build/structs.o build/webserver.o -pthread build/producer.o build/json.o -lm -o producer

build/cons: settings build/tokenizer build/structs build/webserver build/consumer build/json
	${LD} build/tokenizer.o build/structs.o build/webserver.o -pthread build/consumer.o build/json.o -lm -o consumer

build: build/ware build/prod build/cons