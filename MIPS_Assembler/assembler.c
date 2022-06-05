#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdint.h>
#include <ctype.h>
#include <errno.h>

/*
 * For debug option. If you want to debug, set 1.
 * If not, set 0.
 */
#define DEBUG 0

#define MAX_SYMBOL_TABLE_SIZE   1024
#define MEM_TEXT_START          0x00400000
#define MEM_DATA_START          0x10000000
#define BYTES_PER_WORD          4
#define INST_LIST_LEN           20

/******************************************************
 * Structure Declaration
 *******************************************************/

typedef struct inst_struct {
    char *name;
    char *op;
    char type;
    char *funct;
} inst_t;

typedef struct symbol_struct {
    char name[32];
    uint32_t address;
} symbol_t;

enum section { 
    DATA = 0,
    TEXT,
    MAX_SIZE
};

/******************************************************
 * Global Variable Declaration
 *******************************************************/


inst_t inst_list[INST_LIST_LEN] = {       //  idx
    {"addiu",   "001001", 'I', ""},       //    0
    {"addu",    "000000", 'R', "100001"}, //    1
    {"and",     "000000", 'R', "100100"}, //    2
    {"andi",    "001100", 'I', ""},       //    3
    {"beq",     "000100", 'I', ""},       //    4
    {"bne",     "000101", 'I', ""},       //    5
    {"j",       "000010", 'J', ""},       //    6
    {"jal",     "000011", 'J', ""},       //    7
    {"jr",      "000000", 'R', "001000"}, //    8
    {"lui",     "001111", 'I', ""},       //    9
    {"lw",      "100011", 'I', ""},       //   10
    {"nor",     "000000", 'R', "100111"}, //   11
    {"or",      "000000", 'R', "100101"}, //   12
    {"ori",     "001101", 'I', ""},       //   13
    {"sltiu",   "001011", 'I', ""},       //   14
    {"sltu",    "000000", 'R', "101011"}, //   15
    {"sll",     "000000", 'R', "000000"}, //   16
    {"srl",     "000000", 'R', "000010"}, //   17
    {"sw",      "101011", 'I', ""},       //   18
    {"subu",    "000000", 'R', "100011"}  //   19
};


symbol_t SYMBOL_TABLE[MAX_SYMBOL_TABLE_SIZE] = {0}; // Global Symbol Table

uint32_t symbol_table_cur_index = 0; // For indexing of symbol table

/* Temporary file stream pointers */
FILE *data_seg;
FILE *text_seg;

/* Size of each section */
uint32_t data_section_size = 0;
uint32_t text_section_size = 0;

/******************************************************
 * Function Declaration
 *******************************************************/
char* change_file_ext(char *str);
void symbol_table_add_entry(symbol_t symbol);
char* num_to_bits(unsigned int num, int len) ;
int find_symbol(char *label);
int find_index(char text[]);
void record_text_section(FILE *output) ;
void record_data_section(FILE *output);
void make_binary_file(FILE *output);
void make_symbol_table(FILE *input);

/* Change file extension from ".s" to ".o" */
char* change_file_ext(char *str) {
    char *dot = strrchr(str, '.');

    if (!dot || dot == str || (strcmp(dot, ".s") != 0))
        return NULL;

    str[strlen(str) - 1] = 'o';
    return "";
}

/* Add symbol to global symbol table */
void symbol_table_add_entry(symbol_t symbol)
{
    SYMBOL_TABLE[symbol_table_cur_index++] = symbol;
#if DEBUG
    printf("%s: 0x%08x\n", symbol.name, symbol.address);
#endif
}

/* Convert integer number to binary string */
char* num_to_bits(unsigned int num, int len) 
{
    char* bits = (char *) malloc(len+1);
    int idx = len-1, i;
    while (num > 0 && idx >= 0) {
        if (num % 2 == 1) {
            bits[idx--] = '1';
        } else {
            bits[idx--] = '0';
        }
        num /= 2;
    }
    for (i = idx; i >= 0; i--){
        bits[i] = '0';
    }
    bits[len] = '\0';
    return bits;
}

// find index of label
int find_symbol(char* label)
{
    int i;
    for(i=0;i<1024;i++)
        if(strcmp(label,SYMBOL_TABLE[i].name)==0)
            return i;
    return i;
}

//find index of instruction
int find_index(char text[])
{
    int i;
    for(i=0; i<20; i++)
    {
        if(strcmp(text,inst_list[i].name)==0)
            return i;           //index of inst_list
    }
    return i;
}

/* Record .text section to output file */
void record_text_section(FILE *output) 
{
    uint32_t cur_addr = MEM_TEXT_START;
    char line[1024];

    /* Point to text_seg stream */
    rewind(text_seg);

    /* Print .text section */
    while (fgets(line, 1024, text_seg) != NULL) {
        char op[32] = {0};
        char type = '0';
        int i, idx = 0;
        int rs, rt, rd, imm, shamt;
        int addr;

        rs = 0;
        rt = 0;
        rd = 0;
        imm = 0;
        shamt = 0;
        addr = 0;

        char *temp;
        char _line[1024] = {0};
        strcpy(_line, line);
        temp = strtok(_line, "$\t");
        idx = find_index(temp);
        
#if DEBUG
        printf("0x%08x: ", cur_addr);
#endif
        /* Find the instruction type that matches the line */
        /* blank */
	    type = inst_list[idx].type;

        switch (type) {
            case 'R':
            strcpy(op, inst_list[idx].op);
            char function[7];
            strcpy(function, inst_list[idx].funct);
            switch(idx) {
                case 8 : //jr
                    temp = strtok(NULL, "$\t\n");
                    rs = atoi(temp);
                    rt = 0;
                    rd = 0;
                    shamt = 0;
                    break;
                case 16 : //sll
                case 17 : //srl
                    temp = strtok(NULL, "$\t");
                    rd = atoi(temp);

                    temp = strtok(NULL, "$\t");
                    rt = atoi(temp);

                    rs = 0;
                    temp = strtok(NULL, "$\t\n");
                    shamt = atoi(temp);
                    break;
                default : //나머지
                    temp = strtok(NULL, "$\t");
                    rd = atoi(temp);

                    temp = strtok(NULL, "$\t");
                    rs = atoi(temp);

                    temp = strtok(NULL, "$\t\n");
                    rt = atoi(temp);

                    shamt = 0;
                    break;
                }
            fprintf(output, "%s%s%s%s%s%s", op, num_to_bits(rs,5), num_to_bits(rt,5), num_to_bits(rd,5), num_to_bits(shamt,5), function);
#if DEBUG
                printf("op:%s rs:$%d rt:$%d rd:$%d shamt:%d funct:%s\n",
                        op, rs, rt, rd, shamt, inst_list[idx].funct);
#endif
                break;

            case 'I':
                strcpy(op, inst_list[idx].op);
                switch(idx) {
                    case 4 : //beq
                    case 5 : //bne
                        temp = strtok(NULL, "$\t");
                        rs = atoi(temp);

                        temp = strtok(NULL, "$\t");
                        rt = atoi(temp);

                        temp = strtok(NULL, "$\t\n"); 
                        i = find_symbol(temp);
                        imm = SYMBOL_TABLE[i].address;
                        imm = (imm-(cur_addr+4))/4; //offset
                        break;
                    case 9 : //lui
                        temp = strtok(NULL, "$\t");
                        rt = atoi(temp);

                        temp = strtok(NULL, "$\t\n");
                        if (temp[0]=='0')  //16진수
                            imm = strtol(temp,NULL,16);
                        else
                            imm = atoi(temp);
                        rs = 0;
                        break;
                    case 10 : //lw
                    case 18 : //sw
                        temp = strtok(NULL, "$\t");
                        rt = atoi(temp);
                        
                        temp = strtok(NULL, "$\t");
                        imm = atoi(temp);

                        temp = strtok(NULL, "$\t\n");
                        rs = atoi(temp); 
                        break;
                    default : //나머지
                        temp = strtok(NULL, "$\t");
                        rt = atoi(temp);

                        temp = strtok(NULL, "$\t");
                        rs = atoi(temp); 

                        temp = strtok(NULL, "$\t\n");
                        if (temp[0] == '0')  //16진수
                            imm = strtol(temp,NULL,16);
                        else 
                            imm = atoi(temp);
                        break;
                    }
                fprintf(output, "%s%s%s%s", op, num_to_bits(rs,5), num_to_bits(rt,5), num_to_bits(imm,16));
#if DEBUG
                printf("op:%s rs:$%d rt:$%d imm:0x%x\n",
                        op, rs, rt, imm);
#endif
                break;

            case 'J':
                strcpy(op, inst_list[idx].op);

                temp = strtok(NULL, "\t\n");
                i = find_symbol(temp);
                addr = SYMBOL_TABLE[i].address;

                fprintf(output, "%s%s", op, num_to_bits(addr/4,26)); //pc
#if DEBUG
                printf("op:%s addr:%i\n", op, addr);
#endif
                break;

            default:
                break;
        }
        fprintf(output, "\n");

        cur_addr += BYTES_PER_WORD;
    }
}

/* Record .data section to output file */
void record_data_section(FILE *output)
{
    uint32_t cur_addr = MEM_DATA_START;
    char line[1024];

    /* Point to data segment stream */
    rewind(data_seg);

    /* Print .data section */
    while (fgets(line, 1024, data_seg) != NULL) {
        char *temp;
        char _line[1024] = {0};
        strcpy(_line, line);
        temp = strtok(_line, "\t");  //temp = .data
        temp = strtok(NULL, "\t");  //temp = number

        int num;
        if (temp[0] == '0')  //16진수
            num = strtol(temp,NULL,16);
        else    
            num = atoi(temp);
        
        //printf("%s\n",num_to_bits(num,32));
        fprintf(output, "%s\n",num_to_bits(num, 32));
#if DEBUG
        printf("0x%08x: ", cur_addr);
        printf("%s", line);
#endif
        cur_addr += BYTES_PER_WORD;
    }
}

/* Fill the blanks */
void make_binary_file(FILE *output)
{
#if DEBUG
    char line[1024] = {0};
    rewind(text_seg);
    /* Print line of text segment */
    while (fgets(line, 1024, text_seg) != NULL) {
        printf("%s",line);
    }
    printf("text section size: %d, data section size: %d\n",
            text_section_size, data_section_size);
#endif

    /* Print text section size and data section size */
    fprintf(output,"%s\n", num_to_bits(text_section_size,32));
    fprintf(output,"%s\n", num_to_bits(data_section_size,32));
    

    /* Print .text section */
    record_text_section(output);

    /* Print .data section */
    record_data_section(output);
}

/* Fill the blanks */
void make_symbol_table(FILE *input)
{
    char line[1024] = {0};
    uint32_t address_data = MEM_DATA_START;
    uint32_t address_text = MEM_TEXT_START;
    enum section cur_section = MAX_SIZE;

    int i;

    text_seg = tmpfile();
    data_seg = tmpfile();

    /* Read each section and put the stream */
    while (fgets(line, 1024, input) != NULL) {
        char *temp;
        char _line[1024] = {0};
        strcpy(_line, line);
        temp = strtok(_line, "\t\n");

        /* Check section type */
        if (!strcmp(temp, ".data")) {

            cur_section = DATA;
            continue;

        }
        else if (!strcmp(temp, ".text")) {
            
            cur_section = TEXT;
            continue;
            
        }

        /* Put the line into each segment stream */
        if (cur_section == DATA) {
            if(temp == NULL)
                continue;

            if (temp[strlen(temp)-1] == ':') {            //label
                temp[strlen(temp) - 1] = '\0';
                symbol_t label;
                strcpy(label.name, temp);
                label.address = address_data;
                symbol_table_add_entry(label);
                address_data += BYTES_PER_WORD;

                temp = strtok(NULL,"\t\n ");
                if (strcmp(temp, ".word") == 0){       //data_segment            
                    
                    fprintf(data_seg, "%s\t%s\n",temp, strtok(NULL,"\t\n ")); 

                    data_section_size += BYTES_PER_WORD;
                }
                continue;
            }
            else                                        //data_segment    
            {
                fprintf(data_seg, "%s\t%s\n",temp, strtok(NULL,"\t\n "));
                data_section_size += BYTES_PER_WORD;
                address_data += BYTES_PER_WORD;
                continue;
            }
            
        }
        else if (cur_section == TEXT) {

            if (temp[strlen(temp)-1] == ':') {            //label
                temp[strlen(temp) - 1] = '\0';
                symbol_t label;
                strcpy(label.name, temp);
                label.address = address_text;
                symbol_table_add_entry(label);

                continue;
            }
                                                    //text_segment
            if(strcmp(temp,"la")==0)               //LA    
            {
                char *first, *second;
                first = strtok(NULL," ,\t\n");
                second = strtok(NULL," ,\t\n");
                i = find_symbol(second);

                int lui, ori;
                lui = SYMBOL_TABLE[i].address>>16;
                ori = (SYMBOL_TABLE[i].address<<16)>>16;

                fprintf(text_seg, "%s\t%s\t%d\n","lui",first,lui);  //lui
                text_section_size += BYTES_PER_WORD;
                address_text += BYTES_PER_WORD;

                if (ori != 0){
                    fprintf(text_seg, "%s\t%s\t%s\t%d\n","ori",first,first,ori);      //ori
                    text_section_size += BYTES_PER_WORD;
                    address_text += BYTES_PER_WORD;
                }
                continue;
            }

            i = find_index(temp);           //text_segment
            char TYPE = inst_list[i].type;
            char* r1, *r2, *r3;
            switch (TYPE) {
            case 'R':
            switch(i) {
                case 8 :
                    r1 = strtok(NULL, " ,\t\n");
                    fprintf(text_seg, "%s\t%s\n", temp, r1);
                    break;
                default :
                    r1 = strtok(NULL, " ,\t\n");
                    r2 = strtok(NULL, " ,\t\n");
                    r3 = strtok(NULL, " ,\t\n");
                    fprintf(text_seg, "%s\t%s\t%s\t%s\n", temp, r1, r2, r3);
                    break;
                }
                break;
            case 'I':
            switch(i) {
                case 9 :        
                    r1 = strtok(NULL, " \t\n,()");
                    r2 = strtok(NULL, " \t\n,()");
                    fprintf(text_seg, "%s\t%s\t%s\n", temp, r1, r2);
                    break;
                default : 
                    r1 = strtok(NULL, " \t\n,()");
                    r2 = strtok(NULL, " \t\n,()");
                    r3 = strtok(NULL, " \t\n,()");
                    fprintf(text_seg, "%s\t%s\t%s\t%s\n", temp, r1, r2, r3);
                    break;
                    }
                break;
            case 'J':
                r1 = strtok(NULL, " ,\t\n");
                fprintf(text_seg, "%s\t%s\n", temp, r1);
                break;
            }
            text_section_size += BYTES_PER_WORD;
            address_text += BYTES_PER_WORD;

            continue;
        }
    }
}

/******************************************************
 * Function: main
 *
 * Parameters:
 *  int
 *      argc: the number of argument
 *  char*
 *      argv[]: array of a sting argument
 *
 * Return:
 *  return success exit value
 *
 * Info:
 *  The typical main function in C language.
 *  It reads system arguments from terminal (or commands)
 *  and parse an assembly file(*.s).
 *  Then, it converts a certain instruction into
 *  object code which is basically binary code.
 *
 *******************************************************/

int main(int argc, char* argv[])
{
    FILE *input, *output;
    char *filename;

    if (argc != 2) {
        fprintf(stderr, "Usage: %s <*.s>\n", argv[0]);
        fprintf(stderr, "Example: %s sample_input/example?.s\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    /* Read the input file */
    input = fopen(argv[1], "r");
    if (input == NULL) {
        perror("ERROR");
        exit(EXIT_FAILURE);
    }

    /* Create the output file (*.o) */
    filename = strdup(argv[1]); // strdup() is not a standard C library but fairy used a lot.
    if(change_file_ext(filename) == NULL) {
        fprintf(stderr, "'%s' file is not an assembly file.\n", filename);
        exit(EXIT_FAILURE);
    }

    output = fopen(filename, "w");
    if (output == NULL) {
        perror("ERROR");
        exit(EXIT_FAILURE);
    }

    /******************************************************
     *  Let's complete the below functions!
     *
     *  make_symbol_table(FILE *input)
     *  make_binary_file(FILE *output)
     *  ├── record_text_section(FILE *output)
     *  └── record_data_section(FILE *output)
     *
     *******************************************************/
    make_symbol_table(input);
    make_binary_file(output);

    fclose(input);
    fclose(output);

    return 0;
}
