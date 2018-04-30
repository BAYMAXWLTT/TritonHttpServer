#ifndef CALCFSTATEMACHINE_HPP
#define CALCFSTATEMACHINE_HPP

#include <cstdint>

class CalcStateMachine {
public:
  uint64_t update(HttpInstruction instr);

protected:
	// PUT ANY FIELDS/STATE HERE
  uint64_t val = 0;
};

#endif // CALCFRAMER_HPP
