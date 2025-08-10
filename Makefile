CC = gcc
CFLAGS = -I inc -I test
BUILD = build

main_compress: 
	$(CC) $(CFLAGS) src/main.c src/compression.c -o main_compress

test_compression: 
	$(CC) $(CFLAGS) test/unity.c test/test_compression.c src/compression.c -o test_compression

test_array_compaction: 
	$(CC) $(CFLAGS) test/unity.c test/test_array_compaction.c src/compression.c -o test_array_compaction

all: main_compress test_compression test_array_compaction

clean: 
	rm -f main_compress test_compression test_array_compaction