#include <assert.h>
#include <string>
#include <algorithm>
#include <iostream>
#include "CalcParser.hpp"

CalcInstruction CalcParser::parse(std::string insstr)
{
	CalcInstruction ret;

	// PUT YOUR CODE HERE
	insstr.erase(std::remove(insstr.begin(), insstr.end(), ' '), insstr.end());
	std::string instr = insstr.substr(0, 3);
	Instruction parsed_instr = CalcParser::convert(instr);
	switch(parsed_instr){
		case Instruction::ADD:
			ret.calcInstr = Instruction::ADD;
			break;
		case Instruction::SUB:
			ret.calcInstr = Instruction::SUB;
			break;
		case Instruction::SET:
			ret.calcInstr = Instruction::SET;
			break;
		case Instruction::CALC:
			ret.calcInstr = Instruction::CALC;
			break;
	}
	
	if(ret.calcInstr != Instruction::CALC){
			ret.val = std::stoi(insstr.substr(3,insstr.size() - 3),nullptr,10);
	}else{
			ret.val = 0;
	}
	return ret;
}
