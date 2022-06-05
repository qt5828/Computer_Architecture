# Project 1. MIPS Assembler
This project is to implement a MIPS ISA assembler and a given code is a basic skeleton for input and output.

## Introductions
Baiscally, the code written in `assembler.c` operates only creation of file for input and output. Each student requires to implement parsing, converting or any functions based on the skeleton code. The detailed instructions are desribed in `handout/Project_1.pdf`.

There are two directories for this project. The code will read a `.s` file from `sample_input` and it will produce `.o` file in same directory. 

The files in `sample_output` are used when you test your output with them. The following is an example of command for the skeleton code:

```shell
# Usage
$ ./assembler sample_input/example1.s   # This will produce sample_input/example1.o

# Test
$ make test # This will test all examples in sample_output
```

## Description for Skeleton Code
The skeleton code we provided is part of a two-pass assembler. Basically, the two-pass assembler reads the assembly code twice and converts it to binary code (machine code).

![MIPS Assembler](./handout/mips_assembler_processing_example.png)
*MIPS Assembler processing example*

As shown in the figure above, the skeleton code takes an assembly code as an input. Then, in the 1st pass, you need to create a symbol table which specifies a key-value pair to map labels to memory addresses. Here, you are supposed to write code for` make_symbol_table()`. While you are building the symbol table, you may want to extract the `data` and `code` segment separately to simplify the 2nd pass.

In the 2nd pass, the skeleton code just reads the text segement and translates each line of codes into binary (machine code). Here, you are supposed to fill out `make_binary_code()` code. While translating, you may encounter the labels. Now you can translate the labels to the memory addresses by referencing the symbol table you built in the 1st pass. 


## Hints
All students are supposed to implement functions below.

1. __void make_symbol_table(FILE *input)__  
    A role of this function is to make symbol table for converting assembly code to binary.
    This function should be able to parse the line and fill the symbol table. 
    Both data and text line are added to own segment.

2. __void make_binary_file(FILE *output)__  
    Make binary file using the previously created symbol table. 
    You need to print binary lines to the `.o` file(=`FILE *output`) mentioned above.
    You can use `record_text_section()` and `record_data_section`. 
    These functions will print binary code converted from assembly code.
    
3. __void record_text_section(FILE *output)__  
    In this function, you need to translate text segment line by line to binary code. 
    The binary codes should be printed into output file.
    
4. __void record_data_section(FILE *output)__  
    Likewise text section, translates the data segment to binary code.
    It convert their initialized values to binary code and print into output file.

The code that need to be implemented is written as `/* blank */` with short comments. If you do not want to use the skeleton code, it is allowed to write code from scratch.

## Debug
We prepared debug option. It is placed at the top of the code. If you want to enable debug option, define macro `DEBUG` as `1`.
If you want to disable, define macro `DEBUG` as `0`. We hope this option helps you understand the code flow.

## Reference functions
Description of functions used in this project.
1. __FILE* tmpfile(void)__  
    Create a temporary file and return the file stream pointer.
    This function opens the temporary file in wb+ mode.
    
2. __void rewind(FILE *fp)__  
    Point back to the start of the file stream.
 
## Q & A
If you find any errors in this code or have a question, please leave it on AjouBB.
