#ifndef CALCFSTATEMACHINE_HPP
#define CALCFSTATEMACHINE_HPP

#include <cstdint>
#include <vector>
#include "CalcParser.hpp"

class CalcStateMachine {
public:
  uint64_t update(CalcInstruction instr);

protected:
	// PUT ANY FIELDS/STATE HERE
  uint64_t val = 0;
};

#endif // CALCFRAMER_HPP
