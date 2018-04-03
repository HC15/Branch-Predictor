#include "Instruction.h"
#include "Predictor.h"

#include <fstream>
#include <string>
#include <vector>

using namespace std;

int main(int argc, char *argv[]) {
	// check if program has the right amount of command line arguments
	if(argc != 3) {
		return 1; // return 1 to indicate error
	}

	// handle input from file
	ifstream input(argv[1]); // open input file stream to read input

	string line; // string to store lines from input file
	getline(input, line);

	vector<Instruction> trace; // vector to store all the instructions from input file
	while(line != "") { // last line of input is "" so stop there
		trace.push_back(Instruction(line)); // make a Instruction object and put in vector
		getline(input, line); // get the next line
	}
	input.close(); // close the input file stream once done reading

	// calcuate and write to file
	ofstream output(argv[2]); // open output file stream to store ouput

	Predictor predict = Predictor(&trace); // use ptr to save time, dont have to pass entire vector

	// Always Taken and Non-Take
	output << predict.alwaysTaken() << endl;
	output << predict.alwaysNotTaken() << endl;

	// Bimodal Predictor with a single bit of history
	output << predict.oneBitBimodal(16) << ' ';
	output << predict.oneBitBimodal(32) << ' ';
	output << predict.oneBitBimodal(128) << ' ';
	output << predict.oneBitBimodal(256) << ' ';
	output << predict.oneBitBimodal(512) << ' ';
	output << predict.oneBitBimodal(1024) << ' ';
	output << predict.oneBitBimodal(2048) << endl;
	
	// Bimodal Predictor with 2-bit saturating counters
	output << predict.twoBitBimodal(16) << ' ';
	output << predict.twoBitBimodal(32) << ' ';
	output << predict.twoBitBimodal(128) << ' ';
	output << predict.twoBitBimodal(256) << ' ';
	output << predict.twoBitBimodal(512) << ' ';
	output << predict.twoBitBimodal(1024) << ' ';
	output << predict.twoBitBimodal(2048) << endl;	

	// GShare Predictor
	output << predict.gShare(3) << ' ';
	output << predict.gShare(4) << ' ';
	output << predict.gShare(5) << ' ';
	output << predict.gShare(6) << ' ';
	output << predict.gShare(7) << ' ';
	output << predict.gShare(8) << ' ';
	output << predict.gShare(9) << ' ';
	output << predict.gShare(10) << ' ';
	output << predict.gShare(11) << endl;

	// Tournament Predictor
	output << predict.tournament() << endl;

	output.close(); // close output file stream once done writing

	return 0; // return 0 to indicate program executed correctly
}
