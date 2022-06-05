CC = gcc
INC = -I ./
CFLAGS = -std=c99 -g $(INC) -Werror -O3
OBJS = main.o util.o
TARGET = pa0_a

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CC) -o $@ $(OBJS)

.PHONY: clean test help
clean:
	rm -f $(OBJS) $(TARGET)
	rm -f sample_input/example*.out

help:
	@echo "The following options are provided with Make\n\t$$ make \t\t# build main\n\t$$ make clean \t# clean the build\n\t$$ make test \t# test cases"

test: pa0_a test_1 test_2 test_3 test_4 test_5

test_1: $(TARGET)
	@echo "Testing example1"; \
	./$(TARGET) sample_input/example1.in > sample_input/example1.out; \
	diff -Naur sample_output/example1.out sample_input/example1.out; \
	if [ $$? -eq 0 ]; then echo "\tTest seems correct\n"; else echo "\tResults not identical, check the diff output\n"; fi

test_2: $(TARGET)
	@echo "Testing example2"; \
	./$(TARGET) sample_input/example2.in > sample_input/example2.out; \
	diff -Naur sample_output/example2.out sample_input/example2.out; \
	if [ $$? -eq 0 ]; then echo "\tTest seems correct\n"; else echo "\tResults not identical, check the diff output\n"; fi

test_3: $(TARGET)
	@echo "Testing example3"; \
	./$(TARGET) sample_input/example3.in > sample_input/example3.out; \
	diff -Naur sample_output/example3.out sample_input/example3.out; \
	if [ $$? -eq 0 ]; then echo "\tTest seems correct\n"; else echo "\tResults not identical, check the diff output\n"; fi

test_4: $(TARGET)
	@echo "Testing example4"; \
	./$(TARGET) sample_input/example4.in > sample_input/example4.out; \
	diff -Naur sample_output/example4.out sample_input/example4.out; \
	if [ $$? -eq 0 ]; then echo "\tTest seems correct\n"; else echo "\tResults not identical, check the diff output\n"; fi

test_5: $(TARGET)
	@echo "Testing example5"; \
	./$(TARGET) sample_input/example5.in > sample_input/example5.out; \
	diff -Naur sample_output/example5.out sample_input/example5.out; \
	if [ $$? -eq 0 ]; then echo "\tTest seems correct\n"; else echo "\tResults not identical, check the diff output\n"; fi
