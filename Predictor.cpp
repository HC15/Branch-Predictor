#include "Predictor.h"
#include "Instruction.h"

#include <string>
#include <vector>

using namespace std;

Predictor::Predictor(vector<Instruction>* trace) {
	this->trace = trace;
}

string Predictor::result(unsigned int predicted) {
	return to_string(predicted) + ',' + to_string(this->trace->size()) + ';';
}

string Predictor::alwaysTaken() {
	unsigned int taken = 0;

	string outcome;
	for(Instruction instruct : *trace) {
		outcome = instruct.getOutcome();
		
		if(outcome == "T") {
			taken++;
		}
	}

	return this->result(taken);
}

string Predictor::alwaysNotTaken() {
	unsigned int notTaken = 0;

	string outcome;
	for(Instruction instruct : *trace) {
		outcome = instruct.getOutcome();

		if(outcome == "NT") {
			notTaken++;
		}
	}

	return this->result(notTaken);
}

string Predictor::oneBitBimodal(unsigned int size) {
	unsigned int predicted = 0;

	vector<string> table(size, "T");
	unsigned int index;
	string prediction;

	unsigned long address;
	string outcome;
	for(Instruction instruct : *trace) {
		address = instruct.getAddress();
		outcome = instruct.getOutcome();
		
		index = address % size;
		prediction = table[index];

		if(prediction == outcome) {
			predicted++;
		}
		else {
			if(prediction == "T") {
				table[index] = "NT";
			}
			else if(prediction == "NT") {
				table[index] = "T";
			}
		}
	}

	return this->result(predicted);
}

string Predictor::twoBitBimodal(unsigned int size) {
	unsigned int predicted = 0;

	vector<string> table(size, "TT");
	unsigned int index;
	string prediction;
	
	unsigned long address;
	string outcome;
	for(Instruction instruct : *trace) {
		address = instruct.getAddress();
		outcome = instruct.getOutcome();
		
		index = address % size;
		prediction = table[index];

		if(prediction == outcome || 
			(prediction == "TT" && outcome == "T") ||
			(prediction == "NTNT" && outcome == "NT")) {
			predicted++;

			if(prediction == "T") {
				table[index] = "TT";
			}
			else if(prediction == "NT") {
				table[index] = "NTNT";
			}
		}
		else {
			if(prediction == "T") {
				table[index] = "NT";
			}
			else if(prediction == "TT") {
				table[index] = "T";
			}
			else if(prediction == "NT") {
				table[index] = "T";
			}
			else if(prediction == "NTNT") {
				table[index] = "NT";
			}
		}
	}

	return this->result(predicted);
}

string Predictor::gShare(unsigned int size) {
	unsigned int predicted = 0;

	string historyReg(size, '0');
	unsigned long historyValue;

	vector<string> table(2048, "TT");
	unsigned int index;
	string prediction;

	unsigned long address;
	string outcome;
	for(Instruction instruct : *trace) {
		address = instruct.getAddress();
		outcome = instruct.getOutcome();

		historyValue = stoul(historyReg, nullptr, 2);
		index = (address % 2048) ^ historyValue;
		prediction = table[index];

		if(prediction == outcome || 
			(prediction == "TT" && outcome == "T") ||
			(prediction == "NTNT" && outcome == "NT")) {
			predicted++;

			if(prediction == "T") {
				table[index] = "TT";
			}
			else if(prediction == "NT") {
				table[index] = "NTNT";
			}
		}
		else {
			if(prediction == "T") {
				table[index] = "NT";
			}
			else if(prediction == "TT") {
				table[index] = "T";
			}
			else if(prediction == "NT") {
				table[index] = "T";
			}
			else if(prediction == "NTNT") {
				table[index] = "NT";
			}
		}
		
		for(unsigned int i = 0; i < size; i++) {
			if(i < size - 1) {
				historyReg[i] = historyReg[i + 1];
			}
			else {
				if(outcome == "T") {
					historyReg[i] = '1';
				}
				else if(outcome == "NT") {
					historyReg[i] = '0';
				}
			}
		}
	}

	return this->result(predicted);
}

string Predictor::tournament() {
	unsigned int predicted = 0;

	string historyReg(11, '0');
	unsigned long historyValue;

	vector<int> tableSelector(2048, 0);

	vector<string> tableGShare(2048, "TT");
	unsigned long indexGShare;
	string predictionGShare;
	bool predictedGShare;
	
	vector<string> tableBimodal(2048, "TT");
	unsigned long indexBimodal;
	string predictionBimodal;
	bool predictedBimodal;

	unsigned long address;
	string outcome;
	for(Instruction instruct : *trace) {
		address = instruct.getAddress();
		outcome = instruct.getOutcome();

		// gshare
		historyValue = stoul(historyReg, nullptr, 2);
		indexGShare = (address % 2048) ^ historyValue;
		predictionGShare = tableGShare[indexGShare];
		predictedGShare = false;

		if(predictionGShare == outcome || 
			(predictionGShare == "TT" && outcome == "T") ||
			(predictionGShare == "NTNT" && outcome == "NT")) {
			predictedGShare = true;

			if(predictionGShare == "T") {
				tableGShare[indexGShare] = "TT";
			}
			else if(predictionGShare == "NT") {
				tableGShare[indexGShare] = "NTNT";
			}
		}
		else {
			if(predictionGShare == "T") {
				tableGShare[indexGShare] = "NT";
			}
			else if(predictionGShare == "TT") {
				tableGShare[indexGShare] = "T";
			}
			else if(predictionGShare == "NT") {
				tableGShare[indexGShare] = "T";
			}
			else if(predictionGShare == "NTNT") {
				tableGShare[indexGShare] = "NT";
			}
		}
		
		for(int i = 0; i < 11; i++) {
			if(i < 10) {
				historyReg[i] = historyReg[i + 1];
			}
			else {
				if(outcome == "T") {
					historyReg[i] = '1';
				}
				else if(outcome == "NT") {
					historyReg[i] = '0';
				}
			}
		}

		// bimodal
		indexBimodal = address % 2048;
		predictionBimodal = tableBimodal[indexBimodal];
		predictedBimodal = false;

		if(predictionBimodal == outcome || 
			(predictionBimodal == "TT" && outcome == "T") ||
			(predictionBimodal == "NTNT" && outcome == "NT")) {
			predictedBimodal = true;

			if(predictionBimodal == "T") {
				tableBimodal[indexBimodal] = "TT";
			}
			else if(predictionBimodal == "NT") {
				tableBimodal[indexBimodal] = "NTNT";
			}
		}
		else {
			if(predictionBimodal == "T") {
				tableBimodal[indexBimodal] = "NT";
			}
			else if(predictionBimodal == "TT") {
				tableBimodal[indexBimodal] = "T";
			}
			else if(predictionBimodal == "NT") {
				tableBimodal[indexBimodal] = "T";
			}
			else if(predictionBimodal == "NTNT") {
				tableBimodal[indexBimodal] = "NT";
			}
		}

		// selector
		if(predictedGShare && predictedBimodal) {
			predicted++;
		}
		else if(predictedGShare && !predictedBimodal) {
			if(tableSelector[indexBimodal] == 0 ||
				tableSelector[indexBimodal] == 1) {
				predicted++;
			}
			if(tableSelector[indexBimodal] != 0) {
				tableSelector[indexBimodal]--;
			}
		}
		else if(!predictedGShare && predictedBimodal) {
			if(tableSelector[indexBimodal] == 2 ||
				tableSelector[indexBimodal] == 3) {
				predicted++;
			}
			if(tableSelector[indexBimodal] != 3) {
				tableSelector[indexBimodal]++;
			}
		}
	}

	return this->result(predicted);
}
