/***************************************************************/
/*                                                             */
/*   MIPS-32 Instruction Level Simulator                       */
/*                                                             */
/*   SCE212 Ajou University                                    */
/*   parse.h                                                   */
/*   Adapted from CS311@KAIST                                  */
/*                                                             */
/***************************************************************/

/* !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!! */
/*          DO NOT MODIFY THIS FILE!                            */
/*          You should only the parse.c and run.c files!        */
/* !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!! */

#ifndef _PARSE_H_
#define _PARSE_H_

#include <stdio.h>

#include "util.h"

extern int text_size;
extern int data_size;

/* functions */
/** Implement the two parsing_* functions in parse.c */
instruction	parsing_instr(const char *buffer, const int index);
void		parsing_data(const char *buffer, const int index);
void		print_parse_result();

#endif
