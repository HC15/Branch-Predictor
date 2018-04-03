#ifndef PREDICTOR_H
#define PREDICTOR_H

#include "Instruction.h"

#include <string>
#include <vector>

class Predictor {
	private:
		std::vector<Instruction> *trace;

		std::string result(unsigned int predicted);

	public:
		Predictor(std::vector<Instruction>* trace);

		std::string alwaysTaken();
		std::string alwaysNotTaken();
		std::string oneBitBimodal(unsigned int size);
		std::string twoBitBimodal(unsigned int size);
		std::string gShare(unsigned int size);
		std::string tournament();
};

#endif
