#include "Encoder.h"

const string Encoder::defSett1 = "0011";
const string Encoder::defSett2 = "1100";
const string Encoder::defInFilepath = "binaryFile.txt";
const string Encoder::defOutFilepath = "outputFile.txt";
const string Encoder::defDuplFilepath = ".\\output\\duplicates.txt";


Encoder::Encoder(string xorSett1, string xorSett2, string inFilepath, string outFilepath) {


	//set the default xor gate inputs
	EncoderSetting(XOR1REF, xorSett1);
	EncoderSetting(XOR2REF, xorSett2);

	//set file inputs/outputs
	inputFilepath = inFilepath;
	outputFilepath = outFilepath;

	InitialiseVars();	//just to be sure.


}

Encoder::~Encoder() {

	//run clear() on all the vectors just to make sure they're empty
	inputData.clear();
	outputData.clear();
	for (uint i = 0; i < duplEncoders.size(); i++) {
		duplEncoders[i].clear();
	}
	duplEncoders.clear();

}

bool Encoder::RunEncoderPreStep() {
	//read in the data
	if (!ReadInData()) {	//file read in failed!
		cout << "Reading in file failed! Aborting." << endl << endl;
		return false;
	}

	return true;
}

bool Encoder::RunEncoder(bool suppMsg) {
	//run the encoder for inputdata's size +3 to completely flush the data through the registers
	int cycles = inputData.size() + INPUTRANGEDECR;
	for (int i = 0; i < cycles; i++) {
		EncoderCycle();
	}


	//finally output data to file
	if (!WriteOutData()) {	//file write out failed!
		cout << "Write to file failed! Aborting." << endl << endl;
		return false;
	}
	

	//and clean out the variables for cleanliness sake
	InitialiseVars();
	if (!suppMsg) {
		cout << "Encoding completed successfully. File \"" << outputFilepath << "\" has been written to." << endl;
	}

	return true;
}

void Encoder::RunEncoderFullCycle() {
	if (!RunEncoderPreStep()) {
		return;
	}

	cout << "Running..." << endl;

	//loop of (changing output file -> changing gate settings -> encode) to check every setting
	for (int i = 0; i < MAXPERMU; i++) {	//for every gate 1 setting
		for (int j = 0; j < MAXPERMU; j++) {	//for every gate 2 setting
			string gate1Mask = bitset<INPUTRANGE>(i).to_string();	//xor gate number 1's mask
			string gate2Mask = bitset<INPUTRANGE>(j).to_string();	//and gate 2
			string outputName = gate1Mask + "-" + gate2Mask;	//the name of the file to output
			string outputPath = OUTPUTDIR + outputName + FILEEXT;	//and it's directory and extension

			SetOutputPath(outputPath);	//set the output

			EncoderSetting(XOR1REF, gate1Mask);	//set the encoders to the correct masks
			EncoderSetting(XOR2REF, gate2Mask);

			//if it fails, stop the loop
			if (!RunEncoder(true)) {
				cout << "Encoding error! Aborting permutation loop." << endl;
				return;
			}

			//duplicate check
			bool checkIncomp = true;
			//for every encoder that's been generated so far
			for (uint k = 0; k < duplEncoders.size() && checkIncomp; k++) {
				//compare them to the current encoder's output
				if (EncoderCompare(outputPath, OUTPUTDIR + duplEncoders.at(k).at(0) + FILEEXT)) {
					//if they match, add the dupl. encoder's mask to the original's vector
					duplEncoders.at(k).push_back(outputName);
					checkIncomp = false;
				}
			}
			//else add it as a new entry
			if (checkIncomp) {
				vector<string> temp = { outputName };
				duplEncoders.push_back(temp);
			}
			
		}
	}

	//output the duplicate results to file!
	ofstream f(defDuplFilepath.c_str(), ios::out);

	if (!f) {
		cout << "Aw nuts. File output failed." << endl;
		f.close();
	}
	f << "The following file contains sets of encoders that produce duplicate results." << endl << endl;

	//for every encoder and it's associated duplicate encoders
	for (uint i = 0; i < duplEncoders.size(); i++) {
		for (uint j = 0; j < duplEncoders.at(i).size(); j++) {
			f << duplEncoders.at(i).at(j);	//print them out in a group
			if (j != (duplEncoders.at(j).size() - 1)) {
				f << " - ";
			}
		}
		f << endl;
	}

	f.close();


}


void Encoder::EncoderSetting(bool xorNum, string xorSett) {
	for (int i = 0; i < INPUTRANGE; i++) {
		if (xorSett.at(i) == '0') {
			xorInputs[xorNum][i] = false;
		}
		else {
			xorInputs[xorNum][i] = true;
		}
	}

}

void Encoder::SetInputPath(string path) {
	inputFilepath = path;
}
void Encoder::SetOutputPath(string path) {
	outputFilepath = path;
}

bool Encoder::EncoderCompare(string filepath1, string filepath2) {
	ifstream f1stream(filepath1.c_str(), ios::in);
	ifstream f2stream(filepath2.c_str(), ios::in);

	char compChar1;
	char compChar2;

	if (!f1stream || !f2stream) {//Oh dear, it can't find the file :(
		std::cout << "Aw nuts. No file to read in." << endl;
		return false;
	}

	while (f1stream >> compChar1) {	//loop until it returns false (at eof)
		f2stream >> compChar2;
		if (compChar1 != compChar2) {	//if char doesn't match
			return false;
		}
		
	}

	return true;
}


void Encoder::EncoderCycle() {

	//cycle registers and input bit
	RegisterCycle();

	//process xor gates
	XorGate(XOR1REF);
	XorGate(XOR2REF);

	//add to output vector
	for (int i = 0; i < OUTPUTSIZE; i++) {
		outputData.push_back(currentOutput[i]);
	}


}


void Encoder::XorGate(int gateNum) {
	int result = 0;
	//loop through the inputs to the gate and increment the result if active
	for (int i = 0; i < INPUTRANGE; i++) {
		if (xorInputs[gateNum][i])
			result += (int)registerArr[i];
	}

	currentOutput[gateNum] = bool((result % 2) != 0);	//output to currentOutput
}



void Encoder::InitialiseVars() {
	memset(registerArr, 0, sizeof(registerArr));
	memset(currentOutput, 0, sizeof(currentOutput));

	outputData.clear();
	for (uint i = 0; i < duplEncoders.size(); i++) {
		duplEncoders[i].clear();
	}
	duplEncoders.clear();

	inputPos = 0;

}

void Encoder::RegisterCycle() {
	//cycle registers - shift them along
	for (int i = (INPUTRANGE - 1); i > 0; i--) {
		registerArr[i] = registerArr[i - 1];
	}

	//read input bit
	if (inputPos < inputData.size()) {
		registerArr[0] = inputData.at(inputPos);
		inputPos++;
	}
	else {	//else it's hitting the padding at the end, so flush the data with 0's
		registerArr[0] = 0;
	}

}

//reads in data and warns if invalid chars are in the stream
bool Encoder::ReadInData() {

	//clean out current stored data
	inputData.clear();

	//setup input stream
	ifstream f(inputFilepath.c_str(), ios::in);

	if (!f) {//Oh dear, it can't find the file :(
		cout << "Aw nuts. No file to read in." << endl;
		f.close();
		return false;
	}
	char currentChar;

	while (f >> currentChar) {	//loop until it returns false (at eof)
		if (currentChar == '0') {
			inputData.push_back(false);
		}
		else if (currentChar == '1') {
			inputData.push_back(true);
		}
		else {
			cout << "Warning; invalid char in file: " << currentChar << endl;
		}

	}

	f.close();
	cout << "Read in data successfully!" << endl;
	return true;
}

bool Encoder::WriteOutData() {
	//setup input stream
	ofstream f(outputFilepath.c_str(), ios::out);

	if (!f) {
		cout << "Aw nuts. File output failed." << endl;
		f.close();
		return false;
	}

	//take characters from outputData and print to file
	uint dataSize = outputData.size();
	for (uint i = 0; i < dataSize; i++) {
		f << outputData.at(i);
	}

	f.close();
	return true;
}
