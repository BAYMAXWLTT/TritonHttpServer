#include "CalcStateMachine.hpp"
#include "CalcParser.hpp"
#include <cstdint>
#include <iostream>

uint64_t CalcStateMachine::update(CalcInstruction instr){
    switch(instr.calcInstr){
      case Instruction::ADD:
        val += instr.val;
      break;
      case Instruction::SUB:
        val -= instr.val;
      break;
      case Instruction::SET:
        val = instr.val;
      break;
      case Instruction::CALC:
        uint64_t res = val;
        val = 0;
        return res;
      break;
    }

    return 0;
}
