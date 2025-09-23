#include "Lc3.h"
#include <stdio.h>
#include <stdint.h>
#include <signal.h>
/* unix only */
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/time.h>
#include <sys/types.h>
#include <sys/termios.h>
#include <sys/mman.h>
                         
#define Mem_Max (1 << 16) // [0][0][0][0][0][0][0][0][0][0][0][0][0][0][0][1] after shifting [0][0][0][0][0][0][0][0][0][0][0][0][0][0][0][1]
uint16_t memory[Mem_Max]; //   655236 places that everyone of them has 16 bit long; 128 Kb 

enum{                     // regisers are small saving place for cpu and every proccess that cpu do it use this tiny registers 
    R_R0 = 0, 
    R_R1,
    R_R2,
    R_R3,
    R_R4,               // general perpose registers (gpr) this register use for calculating and proccessing values[R0--R7]
    R_R5,
    R_R6,
    R_R7,
    R_PC,               // programm counter register [this register have address of the next instruction to proccess]
    R_COND,
    R_COUNT
};
uint16_t reg[R_COUNT];



enum{
    FL_POS = 1 << 0,
    FL_ZRO = 1 << 1,
    FL_NEG = 1 << 2
};



enum
{
    OP_BR = 0,
    OP_ADD,
    OP_LD,
    OP_ST,
    OP_JSR,
    OP_AND,
    OP_LDR,
    OP_STR,
    OP_RTI,
    OP_NOT,
    OP_LDI,
    OP_STI,
    OP_JMP,
    OP_RES,
    OP_LEA,
    OP_TRAP
};






int main(int argc, char** argv){
    /* if (argc < 2){ */
    /*     /* show usage string */ 
    /*     printf("lc3 [image-file1] ...\n"); */
    /*     exit(2); */
    /* } */
    /**/
    /* for (int j = 1; j < argc; ++j){ */
    /*     if (!read_image(argv[j])){ */
    /*         printf("failed to load image: %s\n", argv[j]); */
    /*         exit(1); */
    /*     } */
    /* } */
    /* } */
    /* signal(SIGINT, handle_interrupt); */
    /* disable_input_buffering() */
    reg[R_COND] = FL_ZRO;

    enum{ PC_START = 0x3000 };
    reg[R_PC] = PC_START;

    int running = 1;
    while(running){
            uint16_t insrt = mem_read(reg[R_PC]++);
            uint16_t op = insrt >> 12;

            switch(op){
                case  OP_ADD:
                 return 0 ;

                case OP_AND :
                    return 0;
            }
    }



}














