/***************************************************************/
/*                                                             */
/*   MIPS-32 Instruction Level Simulator                       */
/*                                                             */
/*   SCE212 Ajou University                                    */
/*   parse.c                                                   */
/*   Adapted from CS311@KAIST                                  */
/*                                                             */
/***************************************************************/

#include <stdio.h>

#include "util.h"
#include "parse.h"

int text_size;
int data_size;


//substring of start_idx-th to end_idx-th index of buffer
char* parse(char* res, char* buffer, int start_idx, int end_idx)
{
    int num = end_idx-start_idx + 1;

    for(int i=0;i<num;i++){
        *res++ = *(buffer+start_idx++);
    }

    res[end_idx-start_idx+1] = NULL;
}

instruction parsing_instr(const char *buffer, const int index)
{
    instruction instr;
	/** Implement this function */
    
    char temp[32], temp2[32];
    uint32_t value;

    //buffer가 const char*->변환
    strcpy(temp2,buffer);

    //value
    value = fromBinary(temp2);
    instr.value = value;

    //opcode
    parse(temp,temp2,0,5);
    instr.opcode = fromBinary(temp);
    
    switch (instr.opcode)
    {
        case 0x0 : // R-type
        //rs
        parse(temp,temp2,6,10);
        instr.r_t.r_i.rs = fromBinary(temp);

        //rt
        parse(temp,temp2,11,15);
        instr.r_t.r_i.rt = fromBinary(temp);

        //rd
        parse(temp,temp2,16,20);
        instr.r_t.r_i.r_i.r.rd = fromBinary(temp);

        //shamt
        parse(temp,temp2,21,25);
        instr.r_t.r_i.r_i.r.shamt = fromBinary(temp);

        //func_code
        parse(temp,temp2,26,31);
        instr.func_code = fromBinary(temp);

        break;

        case 0x2 :  // j-type
        case 0x3 :
        //target
        parse(temp,temp2,6,31);
        instr.r_t.target = fromBinary(temp);

        break;

        default : // i-type
        //rs
        parse(temp,temp2,6,10);
        instr.r_t.r_i.rs = fromBinary(temp);

        //rt
        parse(temp,temp2,11,15);
        instr.r_t.r_i.rt = fromBinary(temp);

        //imm
        parse(temp,temp2,16,31);
        instr.r_t.r_i.r_i.imm = fromBinary(temp);

        break;
    }

    return instr;
    
}

void parsing_data(const char *buffer, const int index)
{
	/** Implement this function */

    char temp[30];
    uint32_t addr, value;

    //buffer가 const char*라서..........
    strcpy(temp, buffer);

    value = fromBinary(temp);
    addr = index + MEM_DATA_START;

    //메모리에 작성(memory update)
    mem_write_32(addr,value);
}

void print_parse_result()
{
    int i;
    printf("Instruction Information\n");

    for(i = 0; i < text_size/4; i++)
    {
        printf("INST_INFO[%d].value : %x\n",i, INST_INFO[i].value);
        printf("INST_INFO[%d].opcode : %d\n",i, INST_INFO[i].opcode);

	    switch(INST_INFO[i].opcode)
        {
            //Type I
            case 0x9:		//(0x001001)ADDIU
            case 0xc:		//(0x001100)ANDI
            case 0xf:		//(0x001111)LUI	
            case 0xd:		//(0x001101)ORI
            case 0xb:		//(0x001011)SLTIU
            case 0x23:		//(0x100011)LW
            case 0x2b:		//(0x101011)SW
            case 0x4:		//(0x000100)BEQ
            case 0x5:		//(0x000101)BNE
                printf("INST_INFO[%d].rs : %d\n",i, INST_INFO[i].r_t.r_i.rs);
                printf("INST_INFO[%d].rt : %d\n",i, INST_INFO[i].r_t.r_i.rt);
                printf("INST_INFO[%d].imm : %d\n",i, INST_INFO[i].r_t.r_i.r_i.imm);
                break;

            //TYPE R
            case 0x0:		//(0x000000)ADDU, AND, NOR, OR, SLTU, SLL, SRL, SUBU  if JR
                printf("INST_INFO[%d].func_code : %d\n",i, INST_INFO[i].func_code);
                printf("INST_INFO[%d].rs : %d\n",i, INST_INFO[i].r_t.r_i.rs);
                printf("INST_INFO[%d].rt : %d\n",i, INST_INFO[i].r_t.r_i.rt);
                printf("INST_INFO[%d].rd : %d\n",i, INST_INFO[i].r_t.r_i.r_i.r.rd);
                printf("INST_INFO[%d].shamt : %d\n",i, INST_INFO[i].r_t.r_i.r_i.r.shamt);
                break;

            //TYPE J
            case 0x2:		//(0x000010)J
            case 0x3:		//(0x000011)JAL
                printf("INST_INFO[%d].target : %d\n",i, INST_INFO[i].r_t.target);
                break;

            default:
                printf("Not available instruction\n");
                assert(0);
        }
    }

    printf("Memory Dump - Text Segment\n");
    for(i = 0; i < text_size; i+=4)
        printf("text_seg[%d] : %x\n", i, mem_read_32(MEM_TEXT_START + i));
    for(i = 0; i < data_size; i+=4)
        printf("data_seg[%d] : %x\n", i, mem_read_32(MEM_DATA_START + i));
    printf("Current PC: %x\n", CURRENT_STATE.PC);
}
