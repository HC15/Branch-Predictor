#ifndef INSTRUCTION_H
#define INSTRUCTION_H

#include <string>

class Instruction {
	private:
		unsigned long address;
		std::string outcome;

	public:
		Instruction(std::string line);

		unsigned long getAddress();
		std::string getOutcome();
};

#endif
