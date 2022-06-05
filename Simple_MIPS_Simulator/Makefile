CC=gcc
CFLAGS=-g -Wall -O0
OBJS=sce212sim.o util.o parse.o run.o
TARGET=sce212sim
SUBMIT = pa2-submission.tar.gz

all: $(TARGET)


$(TARGET): $(OBJS)
	$(CC) -o $@ $(OBJS)

.PHONY: clean test help
clean:
	rm -rf $(OBJS) $(TARGET)

help:
	@echo "The following options are provided with Make\n\t-make:\t\tbuild simulator\n\t-make clean:\tclean the build\n\t-make test:\ttest your simulator"

test: $(TARGET) test_1 test_2 test_3 test_4 test_5 test_fact test_leaf

test_1: $(TARGET)
	@echo "Testing example01"; \
	./$(TARGET) -m 0x10000000:0x10000010 -n 50 sample_input/example01.o | diff -Naur sample_output/example01 - ;\
	if [ $$? -eq 0 ]; then echo "\tTest seems correct\n"; else echo "\tResults not identical, check the diff output\n"; fi

test_2: $(TARGET)
	@echo "Testing example02"; \
	./$(TARGET) -n 50 sample_input/example02.o | diff -Naur sample_output/example02 - ;\
	if [ $$? -eq 0 ]; then echo "\tTest seems correct\n"; else echo "\tResults not identical, check the diff output\n"; fi
test_3: $(TARGET)
	@echo "Testing example03"; \
	./$(TARGET) -n 100 sample_input/example03.o | diff -Naur sample_output/example03 - ;\
	if [ $$? -eq 0 ]; then echo "\tTest seems correct\n"; else echo "\tResults not identical, check the diff output\n"; fi

test_4: $(TARGET)
	@echo "Testing example04"; \
	./$(TARGET) -n 100 sample_input/example04.o | diff -Naur sample_output/example04 - ;\
	if [ $$? -eq 0 ]; then echo "\tTest seems correct\n"; else echo "\tResults not identical, check the diff output\n"; fi

test_5: $(TARGET)
	@echo "Testing example05"; \
	./$(TARGET) -n 100 sample_input/example05.o | diff -Naur sample_output/example05 - ;\
	if [ $$? -eq 0 ]; then echo "\tTest seems correct\n"; else echo "\tResults not identical, check the diff output\n"; fi

test_fact: $(TARGET)
	@echo "Testing fact"; \
	./$(TARGET) -n 100 sample_input/fact.o | diff -Naur sample_output/fact - ;\
	if [ $$? -eq 0 ]; then echo "\tTest seems correct\n"; else echo "\tResults not identical, check the diff output\n"; fi

test_leaf: $(TARGET)
	@echo "Testing leaf_example"; \
	./$(TARGET) -n 100 sample_input/leaf_example.o | diff -Naur sample_output/leaf_example - ;\
	if [ $$? -eq 0 ]; then echo "\tTest seems correct\n"; else echo "\tResults not identical, check the diff output\n"; fi

submission: run.c parse.c
	@echo "Generating a compressed file ($(SUBMIT)) including parse.c and run.c"; \
	tar cvzf $(SUBMIT) run.c parse.c
