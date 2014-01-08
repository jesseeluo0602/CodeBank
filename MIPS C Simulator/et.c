#include <stdio.h> // for stderr
#include <stdlib.h> // for exit()
#include "mips.h" // for execute_syscall()
#include "types.h"

void execute_instruction(Instruction instruction,Processor* processor,Byte *memory) {
    
    /* YOUR CODE HERE: COMPLETE THE SWITCH STATEMENTS */

    sDouble tmp = 0;
    Double tmp2 = 0;
    Word tmp3 = 0;
    switch(instruction.opcode) {
        case 0x0: // opcode == 0x0(SPECIAL)
            switch(instruction.rtype.funct) {
                case 0x00: //sll
                    processor->R[instruction.rtype.rd] = processor->R[instruction.rtype.rt] << instruction.rtype.shamt;
                    processor->PC += 4;
                    break;
                case 0x02: //srl
                    processor->R[instruction.rtype.rd] = processor->R[instruction.rtype.rt] >> instruction.rtype.shamt;
                    processor->PC += 4;
                    break;
                case 0x03: //sra
                    processor->R[instruction.rtype.rd] = ((sWord) processor->R[instruction.rtype.rt]) >> instruction.rtype.shamt;
                    processor->PC += 4;
                    break;
                case 0x08: //jr
                    processor->PC = processor->R[instruction.rtype.rs];
                    break;
                case 0x09: //jalr
                    tmp3 = processor->PC + 4;
                    processor->PC = processor->R[instruction.rtype.rs];
                    processor->R[instruction.rtype.rd] = tmp3;
                    break;
                case 0xc: // funct == 0xc (SYSCALL)
                    execute_syscall(processor);
                    processor->PC += 4;
                    break;
                case 0x10: //mfhi
                    processor->R[instruction.rtype.rd] = processor->RHI;
		            processor->PC += 4;
                    break;
                case 0x12: //mflo
                    processor->R[instruction.rtype.rd] = processor->RLO;
			        processor->PC += 4;
                    break;
                case 0x18: //TODO mult
                    tmp = ((sDouble)(processor->R[instruction.rtype.rs]))*((sDouble)(processor->R[instruction.rtype.rt]));
                    processor->RLO = tmp & 0x00000000ffffffff; 
			        processor->RHI = tmp & 0xffffffff00000000 >> 32;
			        processor->PC += 4;
                    break;
                case 0x19: //TODO multu
                    tmp2 = ((Double)(processor->R[instruction.rtype.rs]))*((Double)(processor->R[instruction.rtype.rt]));
                    processor->RLO = tmp2 & 0x00000000ffffffff; 
			        processor->RHI = tmp2 & 0xffffffff00000000 >> 32;
			        processor->PC += 4;
                    break;
                case 0x21: //addu
                    processor->R[instruction.rtype.rd] = processor->R[instruction.rtype.rs] + processor->R[instruction.rtype.rt];
                    processor->PC += 4;                
                    break;
                case 0x23: //R[rd] ← R[rs] - R[rt] 
                    processor->R[instruction.rtype.rd] = processor->R[instruction.rtype.rs] - processor->R[instruction.rtype.rt];
                    processor->PC += 4;
                    break;
                case 0x24: // funct == 0x24 (AND)
                    processor->R[instruction.rtype.rd] = processor->R[instruction.rtype.rs] & processor->R[instruction.rtype.rt];
                    processor->PC += 4;
                    break;
                case 0x25: //or
                    processor->R[instruction.rtype.rd] = processor->R[instruction.rtype.rs] | processor->R[instruction.rtype.rt];
                    processor->PC += 4;
                    break;
                case 0x26: //xor
                    processor->R[instruction.rtype.rd] = processor->R[instruction.rtype.rs] ^ processor->R[instruction.rtype.rt];
                    processor->PC += 4;
                    break;
                case 0x27: //nor
                    processor->R[instruction.rtype.rd] = ~(processor->R[instruction.rtype.rs] | processor->R[instruction.rtype.rt]);
                    processor->PC += 4;
                    break;
                case 0x2a: //slt
                    if ((sWord)(processor->R[instruction.rtype.rs]) < (sWord)(processor->R[instruction.rtype.rt])) {
                        processor->R[instruction.rtype.rd] = 1;
                    } else {
                        processor->R[instruction.rtype.rd] = 0; 
                    }
                    processor->PC += 4;
                    break;
                case 0x2b: ///sltu
                    if (processor->R[instruction.rtype.rs] < processor->R[instruction.rtype.rt]) {
                        processor->R[instruction.rtype.rd] = 1;
                    } else {
                        processor->R[instruction.rtype.rd] = 0; 
                    }
                    processor->PC += 4;
                    break;
                default: // undefined funct
                    fprintf(stderr,"%s: pc=%08x,illegal function=%08x\n",__FUNCTION__,processor->PC,instruction.bits);
                    exit(-1);
                    break;
            }
            break;
        case 0x2: // opcode == 0x2 (J)
            processor->PC = ((processor->PC+4) & 0xf0000000) | (instruction.jtype.addr << 2);
            break;
        case 0x3: //jal
            processor->R[31] = processor->PC + 4;
		    processor->PC = ((processor->PC+4) & 0xf0000000) | (instruction.jtype.addr << 2);
            break;
        case 0x4: // opcode == 0x4 (beq)
            if (processor->R[instruction.itype.rs] == processor->R[instruction.itype.rt]) {
                processor->PC = (processor->PC + 4) + ((sWord)(sHalf)instruction.itype.imm*4);
            } else {
                processor->PC += 4;
            }
            break;
        case 0x5: // opcode == 0x4 (bne)
            if (processor->R[instruction.itype.rs] != processor->R[instruction.itype.rt]) {
                processor->PC = processor->PC + 4 + ((sWord)(sHalf)instruction.itype.imm*4);
            } else {
                processor->PC += 4;
            }
            break;
        case 0x9://addiu
            processor->R[instruction.itype.rt] = processor->R[instruction.itype.rs] + (sWord)(sHalf)instruction.itype.imm;
      		processor->PC += 4;
            break;
        case 0xa: //slti
            if ((sWord) processor->R[instruction.itype.rs] < (sWord)(sHalf)(instruction.itype.imm)) {
                processor->R[instruction.itype.rt] = 1;
            } else {
                processor->R[instruction.itype.rt] = 0;
            }
            processor->PC +=4;
            break;
        case 0xb: //sltiu
            if (processor->R[instruction.itype.rs] < (sWord)(sHalf)(instruction.itype.imm)) {
                processor->R[instruction.itype.rt] = 1;
            } else {
                processor->R[instruction.itype.rt] = 0;
            }
            processor->PC +=4;
            break;
        case 0xc: //andi
            processor->R[instruction.itype.rt] = processor->R[instruction.itype.rs] & instruction.itype.imm;
            processor->PC += 4;
            break;
        case 0xd: // opcode == 0xd (ORI) 
            processor->R[instruction.itype.rt] = processor->R[instruction.itype.rs] | instruction.itype.imm;
            processor->PC += 4;
            break;
        case 0xe: //xori
            processor->R[instruction.itype.rt] = processor->R[instruction.itype.rs] ^ instruction.itype.imm;
            processor->PC += 4;
            break;
        case 0xf: //lui // 16 bit??
            processor->R[instruction.itype.rt] = (sWord)instruction.itype.imm << 16;
            processor->PC += 4;
            break;
        case 0x20: //lb
            processor->R[instruction.itype.rt] = (sWord)(sByte)(load(memory, (processor->R[instruction.itype.rs] + (sWord)(sHalf)instruction.itype.imm), LENGTH_BYTE));
            processor->PC += 4;
            break;
        case 0x21: //lh
            processor->R[instruction.itype.rt] = (sWord)(sHalf)(load(memory, (processor->R[instruction.itype.rs] + (sWord)(sHalf)instruction.itype.imm), LENGTH_HALF_WORD));
            processor->PC += 4;
            break;
        case 0x23: //lw
            processor->R[instruction.itype.rt] = load(memory, processor->R[instruction.itype.rs] + (sWord)(sHalf)instruction.itype.imm, LENGTH_WORD);
            processor->PC += 4;
            break;
        case 0x24: //lbu
            processor->R[instruction.itype.rt] = load(memory, (processor->R[instruction.itype.rs] + (sWord)(sHalf)instruction.itype.imm), LENGTH_BYTE);
            processor->PC += 4;
            break;
	    case 0x25: //lhu
            processor->R[instruction.itype.rt] = load(memory, (processor->R[instruction.itype.rs] + (sWord)(sHalf)instruction.itype.imm), LENGTH_HALF_WORD);
            processor->PC += 4;
            break;
        case 0x28: //sb
            store(memory, (processor->R[instruction.itype.rs] + (sWord)(sHalf)instruction.itype.imm), LENGTH_BYTE, processor->R[instruction.itype.rt]);
            processor->PC += 4;
            break;
        case 0x29: //sh
            store(memory, (processor->R[instruction.itype.rs] + (sWord)(sHalf)instruction.itype.imm), LENGTH_HALF_WORD, processor->R[instruction.itype.rt]);
            processor->PC += 4;
            break;
        case 0x2b: //sw
            store(memory, (processor->R[instruction.itype.rs] + (sWord)(sHalf)instruction.itype.imm), LENGTH_WORD, processor->R[instruction.itype.rt]);
            processor->PC += 4;
            break;
        default: // undefined opcode
            fprintf(stderr,"%s: pc=%08x,illegal instruction: %08x\n",__FUNCTION__,processor->PC,instruction.bits);
            exit(-1);
            break;
    }
}


int check(Address address,Alignment alignment) {

    /* YOUR CODE HERE */
    if ((address % alignment == 0) && (address <= MEMORY_SPACE) && (address >0)) { //
    	return 1;
    } 
    return 0;
}

void store(Byte *memory,Address address,Alignment alignment,Word value) {
    if(!check(address,alignment)) {
        fprintf(stderr,"%s: bad write=%08x\n",__FUNCTION__,address);
        exit(-1);
    }
    
    /* YOUR CODE HERE */
    int i;
    for (i=0; i<alignment; i++) {
    	*(address + memory+ i) = (value & 0xFF);
    	value = value >> 8;
    }
}

Word load(Byte *memory,Address address,Alignment alignment) {
    if(!check(address,alignment)) {
        fprintf(stderr,"%s: bad read=%08x\n",__FUNCTION__,address);
        exit(-1);
    }
    
    /* YOUR CODE HERE */
    if (alignment==LENGTH_BYTE) {
    	return *(Byte*)(memory + address);
    }
    else if (alignment==LENGTH_HALF_WORD) {
    	return *(Half*)(memory + address);
    }
    // incomplete stub to let "simple" execute
    // (only handles size == SIZE_WORD correctly)
    // feel free to delete and implement your own way
    return *(Word*)(memory+address);
}