#include "Instruction.h"

#include <string>

using namespace std;

Instruction::Instruction(string line) {
	this->address = stoul(line.substr(0, 10), nullptr, 16);
	this->outcome = line.substr(11, 2);
}

unsigned long Instruction::getAddress() {
	return this->address;
}

string Instruction::getOutcome() {
	return this->outcome;
}
