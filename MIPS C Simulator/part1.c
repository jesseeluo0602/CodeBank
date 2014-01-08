#include <stdio.h> // for stderr
#include <stdlib.h> // for exit()
#include "types.h"

void decode_instruction(Instruction instruction) {
    
    /* YOUR CODE HERE: COMPLETE THE SWITCH STATEMENTS */
    switch(instruction.opcode) {
        case 0x0: // opcode == 0x0 (SPECIAL)
            switch(instruction.rtype.funct) {
                case 0x00: // funct == 0x00 (SLL)
                    printf("sll\t$%d,$%d,%u\n",instruction.rtype.rd,instruction.rtype.rt,instruction.rtype.shamt);
                    break;    
                case 0x02: // funct == 0x02 (SRL)
                    printf("srl\t$%d,$%d,%u\n",instruction.rtype.rd,instruction.rtype.rt,instruction.rtype.shamt);
                    break;
                case 0x03: // funct == 0x03 (SRA)
                    printf("sra\t$%d,$%d,%u\n",instruction.rtype.rd,instruction.rtype.rt,instruction.rtype.shamt);
                    break;      
                case 0x08: // funct == 0x08 (JR)
                    printf("jr\t$%d\n",instruction.rtype.rs);
                    break;    
                case 0x09: // funct == 0x09 (JALR)
                    printf("jalr\t$%d,$%d\n",instruction.rtype.rd,instruction.rtype.rs);
                    break;                                         
                case 0xc: // funct == 0xc (SYSCALL)
                    printf("syscall\n");
                    break;
                case 0x10: // funct == 0x10 (MFHI)
                    printf("mfhi\t$%d\n",instruction.rtype.rd);
                    break;
                case 0x12: // funct == 0x12 (MFLO)
                    printf("mflo\t$%d\n",instruction.rtype.rd);
                    break;
                case 0x18: // funct == 0x18 (MULT)
                    printf("mult\t$%d,$%d\n",instruction.rtype.rs,instruction.rtype.rt);
                    break; 
                case 0x19: // funct == 0x19 (MULTU)
                    printf("multu\t$%d,$%d\n",instruction.rtype.rs,instruction.rtype.rt);
                    break;           
                case 0x21: // funct == 0x21 (ADDU)
                    printf("addu\t$%d,$%d,$%d\n",instruction.rtype.rd,instruction.rtype.rs,instruction.rtype.rt);
                    break;  
                case 0x23: // funct == 0x23 (SUBU)
                    printf("subu\t$%d,$%d,$%d\n",instruction.rtype.rd,instruction.rtype.rs,instruction.rtype.rt);
                    break;                   
                case 0x24: // funct == 0x24 (AND)
                    printf("and\t$%d,$%d,$%d\n",instruction.rtype.rd,instruction.rtype.rs,instruction.rtype.rt);
                    break;
                case 0x25: // funct == 0x25 (OR)
                    printf("or\t$%d,$%d,$%d\n",instruction.rtype.rd,instruction.rtype.rs,instruction.rtype.rt);
                    break;
                case 0x26: // funct == 0x26 (XOR)
                    printf("xor\t$%d,$%d,$%d\n",instruction.rtype.rd,instruction.rtype.rs,instruction.rtype.rt);
                    break;
                case 0x27: // funct == 0x27 (NOR)
                    printf("nor\t$%d,$%d,$%d\n",instruction.rtype.rd,instruction.rtype.rs,instruction.rtype.rt);
                    break;
                case 0x2a: // funct == 0x2a (SLT)
                    printf("slt\t$%d,$%d,$%d\n",instruction.rtype.rd,instruction.rtype.rs,instruction.rtype.rt);
                    break;
                case 0x2b: // funct == 0x2b (SLTU)
                    printf("sltu\t$%d,$%d,$%d\n",instruction.rtype.rd,instruction.rtype.rs,instruction.rtype.rt);
                    break;
                default: // undefined funct
                    fprintf(stderr,"%s: illegal function: %08x\n",__FUNCTION__,instruction.bits);
                    exit(-1);
                    break;
            }
            break;
        case 0x2: // opcode == 0x2 (J)
            printf("j\t0x%x\n",instruction.jtype.addr*4);
            break;
        case 0x3: // opcode == 0x3 (JAL)
            printf("jal\t0x%x\n",instruction.jtype.addr*4);
            break;
        case 0x4: // opcode == 0x04 (BEQ)
            printf("beq\t$%d,$%d,%d\n",instruction.itype.rs,instruction.itype.rt,(sHalf)instruction.itype.imm*4);
            break;
        case 0x5: // opcode == 0x05 (BNE)
            printf("bne\t$%d,$%d,%d\n",instruction.itype.rs,instruction.itype.rt,(sHalf)instruction.itype.imm*4);
            break;
        case 0x9: // opcode == 0x09 (ADDIU)
            printf("addiu\t$%d,$%d,%d\n",instruction.itype.rt,instruction.itype.rs,(sHalf)instruction.itype.imm);
            break;
        case 0xa: // opcode == 0x0a (SLTI)
            printf("slti\t$%d,$%d,%d\n",instruction.itype.rt,instruction.itype.rs,(sHalf)instruction.itype.imm);
            break;
        case 0xb: // opcode == 0x0b (SLTIU)
            printf("sltui\t$%d,$%d,%d\n",instruction.itype.rt,instruction.itype.rs,(sHalf)instruction.itype.imm);
            break;
        case 0xc: // opcode == 0x0c (ANDI)
            printf("andi\t$%d,$%d,0x%x\n",instruction.itype.rt,instruction.itype.rs,instruction.itype.imm);
            break;
        case 0xd: // opcode == 0xd (ORI)
            printf("ori\t$%d,$%d,0x%x\n",instruction.itype.rt,instruction.itype.rs,instruction.itype.imm);
            break;
        case 0xe: // opcode == 0x0e (XORI)
            printf("xori\t$%d,$%d,0x%x\n",instruction.itype.rt,instruction.itype.rs,instruction.itype.imm);
            break; 
        case 0xf: // opcode == 0x0f (LUI)
            printf("lui\t$%d,0x%x\n",instruction.itype.rt,instruction.itype.imm);
            break;
        case 0x20: // opcode == 0x20 (LB)
            printf("lb\t$%d,%d($%d)\n",instruction.itype.rt,(sHalf)instruction.itype.imm,instruction.itype.rs);
            break;
        case 0x21: // opcode == 0x21 (LH)
            printf("lh\t$%d,%d($%d)\n",instruction.itype.rt,(sHalf)(sHalf)(sHalf)instruction.itype.imm,instruction.itype.rs);
            break;  
        case 0x23: // opcode == 0x23 (LW)
            printf("lw\t$%d,%d($%d)\n",instruction.itype.rt,(sHalf)(sHalf)instruction.itype.imm,instruction.itype.rs);
            break;  
        case 0x24: // opcode == 0x24 (LBU)
            printf("lbu\t$%d,%d($%d)\n",instruction.itype.rt,(sHalf)instruction.itype.imm,instruction.itype.rs);
            break;
        case 0x25: // opcode == 0x25 (LHU)
            printf("lhu\t$%d,%d($%d)\n",instruction.itype.rt,(sHalf)(sHalf)(sHalf)(sHalf)instruction.itype.imm,instruction.itype.rs);
            break;
        case 0x28: // opcode == 0x28 (SB)
            printf("sb\t$%d,%d($%d)\n",instruction.itype.rt,(sHalf)(sHalf)(sHalf)instruction.itype.imm,instruction.itype.rs);
            break;
        case 0x29: // opcode == 0x29 (SH)
            printf("sh\t$%d,%d($%d)\n",instruction.itype.rt,(sHalf)(sHalf)instruction.itype.imm,instruction.itype.rs);
            break;
        case 0x2b: // opcode == 0x2b (SW)
            printf("sw\t$%d,%d($%d)\n",instruction.itype.rt,(sHalf)instruction.itype.imm,instruction.itype.rs);
            break;
        default: // undefined opcode
            fprintf(stderr,"%s: illegal instruction: %08x\n",__FUNCTION__,instruction.bits);
            exit(-1);
            break;
    }
}
