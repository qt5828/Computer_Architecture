CC=gcc
CFLAGS=-g -Wall -std=gnu99 -O3
OBJS=assembler.o
TARGET=assembler

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CC) -o $@ $(OBJS)

.PHONY: clean test help

clean:
	rm -rf *.o $(TARGET)
	rm -rf *.o sample_input/*.o

help:
	@echo "The following options are provided with Make\n\t$$ make \t\t# build assembler\n\t$$ make clean \t# clean the build\n\t$$ make test \t# test cases"

test: $(TARGET) test_1 test_2 test_3 test_4 test_5 test_6 test_7

test_1: $(TARGET)
	@echo "Testing example1"; \
	./$(TARGET) sample_input/example1.s; \
	diff -Naur sample_input/example1.o sample_output/example1.o; \
	if [ $$? -eq 0 ]; then echo "\tTest seems correct\n"; else echo "\tResults not identical, check the diff output\n"; fi

test_2: $(TARGET)
	@echo "Testing example2_mod"; \
	./$(TARGET) sample_input/example2_mod.s; \
	diff -Naur sample_input/example2_mod.o sample_output/example2_mod.o; \
	if [ $$? -eq 0 ]; then echo "\tTest seems correct\n"; else echo "\tResults not identical, check the diff output\n"; fi

test_3: $(TARGET)
	@echo "Testing example3"; \
	./$(TARGET) sample_input/example3.s; \
	diff -Naur sample_input/example3.o sample_output/example3.o; \
	if [ $$? -eq 0 ]; then echo "\tTest seems correct\n"; else echo "\tResults not identical, check the diff output\n"; fi

test_4: $(TARGET)
	@echo "Testing example4"; \
	./$(TARGET) sample_input/example4.s; \
	diff -Naur sample_input/example4.o sample_output/example4.o; \
	if [ $$? -eq 0 ]; then echo "\tTest seems correct\n"; else echo "\tResults not identical, check the diff output\n"; fi

test_5: $(TARGET)
	@echo "Testing example5"; \
	./$(TARGET) sample_input/example5.s; \
	diff -Naur sample_input/example5.o sample_output/example5.o; \
	if [ $$? -eq 0 ]; then echo "\tTest seems correct\n"; else echo "\tResults not identical, check the diff output\n"; fi

test_6: $(TARGET)
	@echo "Testing example6"; \
	./$(TARGET) sample_input/example6.s; \
	diff -Naur sample_input/example6.o sample_output/example6.o; \
	if [ $$? -eq 0 ]; then echo "\tTest seems correct\n"; else echo "\tResults not identical, check the diff output\n"; fi

test_7: $(TARGET)
	@echo "Testing example7"; \
	./$(TARGET) sample_input/example7.s; \
	diff -Naur sample_input/example7.o sample_output/example7.o; \
	if [ $$? -eq 0 ]; then echo "\tTest seems correct\n"; else echo "\tResults not identical, check the diff output\n"; fi
