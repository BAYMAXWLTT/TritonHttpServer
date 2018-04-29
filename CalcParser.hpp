#ifndef CALCPARSER_HPP
#define CALCPARSER_HPP

#include <string>
#include <cstdint>
#include <fstream>
using namespace std;

enum Instruction{
	ADD,
	SUB,
	SET,
	CALC
};

typedef struct CalcInstruction_t {
	// DEFINE YOUR DATA STRUCTURE HERE
	Instruction calcInstr;
	uint64_t val;
} CalcInstruction;

class CalcParser {
public:
	static CalcInstruction parse(string insstr);
private:
	static Instruction convert(string insstr){
		if(insstr.compare("ADD") == 0){
			return ADD;
		}else if(insstr.compare("SUB") == 0){
			return SUB;
		}else if(insstr.compare("SET") == 0){
			return SET;
		}else{
			return CALC;
		}
	}
};

#endif // CALCPARSER_HPP
