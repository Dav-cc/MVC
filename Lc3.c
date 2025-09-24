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

void update_flage(uint16_t r);
uint16_t sign_extend(uint16_t x, int bit_count);

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
    FL_POS = 1 << 0,        // 0000000000000001
    FL_ZRO = 1 << 1,        // 0000000000000010
    FL_NEG = 1 << 2         // 0000000000000100
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
    /*      show usage string */ 
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
        uint16_t instr = mem_read(reg[R_PC]++);
        uint16_t op = instr >> 12;

        switch(op){
            case  OP_ADD:{
                // destnation register
                uint16_t r0 = (instr >> 9) & 0x7;

                //source register(SR1)
                uint16_t r1 = (instr << 6) & 0x7;

                // flage (bit 5 of instruction set)
                uint16_t imm_flag = (instr << 5) & 0x1;

                if(imm_flag){
                    uint16_t imm5 = sign_extend(instr & 0x1F, 5);
                    reg[r0] = reg[r1] + imm5;
                }
                else{
                    uint16_t r2 = instr & 0x7;
                    reg[r0] = reg[r1] + reg[r2];
                }
                update_flage(r0);
            }
            case OP_LDI :{
                uint16_t r0 = (instr >> 9) & 0x7 ;
                uint16_t pcoffset9 = sign_extend(instr & 0x1FF, 9);
                reg[r0] = mem_read(mem_read(reg[R_PC] + pcoffset9));
                update_flage(r0);
            }
            case OP_AND :{
                uint16_t r0 = instr >> 9 & 0x7;
                uint16_t r1 = instr >> 6 & 0x7;
                uint16_t flag = instr >> 5 & 0x1;

                if(flag){
                    uint16_t imm5 = sign_extend(instr & 0x1F, 5);
                    reg[r0] = reg[r1] & imm5;
                }
                else{
                    uint16_t r2 = instr & 0x7; 
                    reg[r0] = reg[r1] & reg[r2];
                }
                update_flage(r0);
            }
            case OP_NOT :{
                uint16_t r0 = instr >> 9 & 0x7; 
                uint16_t r1 = instr >> 6 & 0x7; 
                reg[r0] = ~reg[r1];
                update_flage(r0);
            }
            case OP_BR :{
                uint16_t cond_flag = instr >> 6 & 0x7;
                uint16_t offset = sign_extend(instr >> 8 & 0x1FF, 9);
                if(cond_flag & reg[R_COND]){
                    reg[R_PC] += offset;
                }
            }
            case OP_JMP:{
                uint16_t r1 = (instr >> 6) & 0x7;
                reg[R_PC] = reg[r1];
            }
            case OP_JSR:{
                uint16_t long_flag = (instr >> 11) & 1;
                reg[R_R7] = reg[R_PC];
                if(long_flag){
                    uint16_t pcoffset = sign_extend(instr & 0x7FF, 11);
                    reg[R_PC] += pcoffset;  // JSR
                }
                else{
                    uint16_t r1 = (instr >> 6) & 0x7;
                    reg[R_PC] = reg[r1]; /* JSRR */
                }
            }
            case OP_LD:{
                uint16_t r0 = (instr >> 9) & 0x7;
                uint16_t pc_offset = sign_extend(instr & 0x1FF, 9);
                reg[r0] = mem_read(reg[R_PC] + pc_offset);
                update_flage(r0);
            }
            case OP_LDR:{
                uint16_t r0 = (instr >> 9) & 0x7;
                uint16_t r1 = (instr >> 6) & 0x7;
                uint16_t offset = sign_extend(instr & 0x3F, 6);
                reg[r0] = mem_read(reg[r1] + offset);
                update_flage(r0);
            }
            case OP_LEA:{
                uint16_t r0 = (instr >> 9) & 0x7;
                uint16_t pc_offset = sign_extend(instr & 0x1FF, 9);
                reg[r0] = reg[R_PC] + pc_offset;
                update_flage(r0);
            }
            case OP_STI:{
                uint16_t r0 = (instr >> 9) & 0x7;
                uint16_t pc_offset = sign_extend(instr & 0x1FF, 9);
                mem_write(mem_read(reg[R_PC] + pc_offset), reg[r0]);
            }
            case OP_STR:{
                    uint16_t r0 = (instr >> 9) & 0x7;
                    uint16_t r1 = (instr >> 6) & 0x7;
                    uint16_t offset = sign_extend(instr & 0x3F, 6);
                    mem_write(reg[r1] + offset, reg[r0]);
            }
        }
    }
}

uint16_t sign_extend(uint16_t x, int bit_count){        // convert negative binary and posetive binary numbers to 16 bits
    if((x >> (bit_count - 1)) & 1){
        x |= (0xFFFF2 << bit_count);
    }
    return x;
}

void update_flage(uint16_t r){
    if(reg[r] == 0){
        reg[R_COND] = FL_ZRO;
    }
    else if(reg[r] << 15){
        reg[R_COND] = FL_NEG;
    }
    else{
        reg[R_COND] = FL_POS;
    }
}
