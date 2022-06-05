/***************************************************************/
/*                                                             */
/*   MIPS-32 Instruction Level Simulator                       */
/*                                                             */
/*   SCE212 Ajou University                                    */
/*   run.c                                                     */
/*   Adapted from CS311@KAIST                                  */
/*                                                             */
/***************************************************************/

#include <stdio.h>

#include "util.h"
#include "run.h"

/***************************************************************/
/*                                                             */
/* Procedure: get_inst_info                                    */
/*                                                             */
/* Purpose: Read insturction information                       */
/*                                                             */
/***************************************************************/
instruction* get_inst_info(uint32_t pc)
{
    return &INST_INFO[(pc - MEM_TEXT_START) >> 2];
}

/***************************************************************/
/*                                                             */
/* Procedure: process_instruction                              */
/*                                                             */
/* Purpose: Process one instrction                             */
/*                                                             */
/***************************************************************/

void process_instruction()
{
	/* Implement this function */

    uint32_t PC = CURRENT_STATE.PC;
    instruction* inst = get_inst_info(PC);
    int n_th; //n번째 inst.=>(현재)

    //PC update
    PC += 4;

    //register update +(memory update)
    switch (inst->opcode)
    {
        case 0x0 : // R-type
        switch(inst->func_code)
        {
            case 0x21 : //addu
            CURRENT_STATE.REGS[inst->r_t.r_i.r_i.r.rd] = CURRENT_STATE.REGS[inst->r_t.r_i.rs] + CURRENT_STATE.REGS[inst->r_t.r_i.rt]; 
            break;
            case 0x24 : //and
            CURRENT_STATE.REGS[inst->r_t.r_i.r_i.r.rd] = CURRENT_STATE.REGS[inst->r_t.r_i.rs] & CURRENT_STATE.REGS[inst->r_t.r_i.rt]; 
            break;
            case 0x8 : //jr
            PC = CURRENT_STATE.REGS[inst->r_t.r_i.rs];
            break;
            case 0x27 : //nor
            CURRENT_STATE.REGS[inst->r_t.r_i.r_i.r.rd] = ~(CURRENT_STATE.REGS[inst->r_t.r_i.rs] | CURRENT_STATE.REGS[inst->r_t.r_i.rt]); 
            break;
            case 0x25 : //or
            CURRENT_STATE.REGS[inst->r_t.r_i.r_i.r.rd] = CURRENT_STATE.REGS[inst->r_t.r_i.rs] | CURRENT_STATE.REGS[inst->r_t.r_i.rt]; 
            break;
            case 0x2b : //sltu
            if (CURRENT_STATE.REGS[inst->r_t.r_i.rs] < CURRENT_STATE.REGS[inst->r_t.r_i.rt])
                CURRENT_STATE.REGS[inst->r_t.r_i.r_i.r.rd] = 1;
            else 
                CURRENT_STATE.REGS[inst->r_t.r_i.r_i.r.rd] = 0;
            break;
            case 0x0 : //sll
            CURRENT_STATE.REGS[inst->r_t.r_i.r_i.r.rd] = CURRENT_STATE.REGS[inst->r_t.r_i.rt] << inst->r_t.r_i.r_i.r.shamt; 
            break;
            case 0x2 : //srl
            CURRENT_STATE.REGS[inst->r_t.r_i.r_i.r.rd] = CURRENT_STATE.REGS[inst->r_t.r_i.rt] >> inst->r_t.r_i.r_i.r.shamt; 
            break;
            case 0x23 : //subu
            CURRENT_STATE.REGS[inst->r_t.r_i.r_i.r.rd] = CURRENT_STATE.REGS[inst->r_t.r_i.rs] - CURRENT_STATE.REGS[inst->r_t.r_i.rt]; 
            break;
        }
        break;

        // I-type
        case 0x9 : //addiu
        CURRENT_STATE.REGS[inst->r_t.r_i.rt] = CURRENT_STATE.REGS[inst->r_t.r_i.rs] + inst->r_t.r_i.r_i.imm;
        break;
        case 0xc : //andi
        CURRENT_STATE.REGS[inst->r_t.r_i.rt] = CURRENT_STATE.REGS[inst->r_t.r_i.rs] & inst->r_t.r_i.r_i.imm;
        break;
        case 0x4 : //beq
        if (CURRENT_STATE.REGS[inst->r_t.r_i.rt] == CURRENT_STATE.REGS[inst->r_t.r_i.rs])
            PC = PC + (inst->r_t.r_i.r_i.imm)*4; //Target addr=PC+offset*4
        break;
        case 0x5 : //bne
        if (CURRENT_STATE.REGS[inst->r_t.r_i.rt] != CURRENT_STATE.REGS[inst->r_t.r_i.rs])
            PC = PC + (inst->r_t.r_i.r_i.imm)*4; //Target addr=PC+offset*4
        break;
        case 0xf : //lui
        CURRENT_STATE.REGS[inst->r_t.r_i.rt] = (inst->r_t.r_i.r_i.imm) << 16; //반대인가?-> test해보기
        break;
        case 0x23 : //lw
        CURRENT_STATE.REGS[inst->r_t.r_i.rt] = mem_read_32(CURRENT_STATE.REGS[inst->r_t.r_i.rs] + inst->r_t.r_i.r_i.imm);
        break;
        case 0xd : //ori
        CURRENT_STATE.REGS[inst->r_t.r_i.rt] = CURRENT_STATE.REGS[inst->r_t.r_i.rs] | inst->r_t.r_i.r_i.imm;
        break;
        case 0xb : //sltiu
        if (CURRENT_STATE.REGS[inst->r_t.r_i.rs] < inst->r_t.r_i.r_i.imm)
            CURRENT_STATE.REGS[inst->r_t.r_i.rt] = 1;
        else
            CURRENT_STATE.REGS[inst->r_t.r_i.rt] = 0;
        break;
        case 0x2b : //sw
        mem_write_32(CURRENT_STATE.REGS[inst->r_t.r_i.rs] + inst->r_t.r_i.r_i.imm, CURRENT_STATE.REGS[inst->r_t.r_i.rt]);
        break;


        // J-type
        case 0x2 : //j
        PC = (inst->r_t.target)*4;  //PC=add*4
        break;
        case 0x3 : //jal
        CURRENT_STATE.REGS[31] = PC + 4;
        PC = (inst->r_t.target)*4;   //PC=add*4
        break;

    }

    //변경된 PC update
    CURRENT_STATE.PC = PC;
    
    //현재 inst. 개수(n번째)
    n_th = (CURRENT_STATE.PC - MEM_TEXT_START) / 4;

    //끝이면.
    if (n_th == NUM_INST)
        RUN_BIT = FALSE;


}
