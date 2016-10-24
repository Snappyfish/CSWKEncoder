#include "Encoder.h"

const string Encoder::defSett1 = "0011";
const string Encoder::defSett2 = "1100";
const string Encoder::defInFilepath = "binaryFile.txt";
const string Encoder::defOutFilepath = "outputFile.txt";


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
	//then actually encode the data
	int cycles = inputData.size();
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

	//loop of (changing output file -> changing gate settings -> encode) to check every setting
	for (int i = 0; i < MAXPERMU; i++) {	//for every gate 1 setting
		for (int j = 0; j < MAXPERMU; j++) {	//for every gate 2 setting
			string gate1Mask = bitset<4>(j).to_string();
			string gate2Mask = bitset<4>(i).to_string();

			SetOutputPath(OUTPUTDIR + gate1Mask + "-" + gate2Mask + FILEEXT);

			EncoderSetting(XOR1REF, gate1Mask);
			EncoderSetting(XOR2REF, gate2Mask);

			//if it fails, stop the loop
			if (!RunEncoder(true)) {
				cout << "Encoding error! Aborting permutation loop." << endl;
				return;
			}

		}
	}



}


void Encoder::EncoderSetting(bool xorNum, string xorSett) {
	if (xorNum) {
		for (int i = 0; i < INPUTRANGE; i++) {
			char temp = xorSett.at(i);
			if (temp == '0') {
				xor1Inputs[i] = false;
			}
			else {
				xor1Inputs[i] = true;
			}
		}
	}
	else {
		for (int i = 0; i < INPUTRANGE; i++) {
			char temp = xorSett.at(i);
			if (temp == '0') {
				xor2Inputs[i] = false;
			}
			else {
				xor2Inputs[i] = true;
			}
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

	if (!f1stream || !f2stream) {//Oh dear, it can't find the file :(
		std::cout << "Aw nuts. No file to read in." << endl;
		return false;
	}

	while (f1stream >> compChar1) {	//loop until it returns false (at eof)
		if (f2stream >> compChar2) {	//if the char is read in properly
			if (compChar1 != compChar2) {	//if char doesn't match
				return false;
			}
		}
		else {
			return false;
		}
		



	}

	return true;
}


void Encoder::EncoderCycle() {

	//cycle registers and input bit
	RegisterCycle();

	//process xor gates
	XorGate1();
	XorGate2();

	//add to output vector
	for (int i = 0; i < OUTPUTSIZE; i++) {
		outputData.push_back(currentOutput[i]);
	}


}


void Encoder::XorGate1() {
	int result = 0;
	for (int i = 0; i < INPUTRANGE; i++) {
		if (xor1Inputs[i])
			result += (int)registerArr[i];
	}

	currentOutput[0] = bool(result % 2);

}
//code reuse? \/
void Encoder::XorGate2() {
	int result = 0;
	for (int i = 0; i < INPUTRANGE; i++) {
		if (xor2Inputs[i])
			result += registerArr[i];
	}

	currentOutput[1] = bool(result % 2);

}




void Encoder::InitialiseVars() {
	memset(registerArr, 0, sizeof(registerArr));
	memset(currentOutput, 0, sizeof(currentOutput));

	//inputData.clear();
	outputData.clear();
	inputPos = 0;

}

void Encoder::RegisterCycle() {
	//cycle registers
	for (int i = (INPUTRANGE - 1); i > 0; i--) {
		registerArr[i] = registerArr[i - 1];
	}

	//read input bit
	if (inputPos < inputData.size()) {
		registerArr[0] = inputData[inputPos];
		inputPos++;
	}
	else {	//else it's hitting the padding at the end, so flush the data with 0's
		registerArr[0] = 0;
	}

}

//adapted from my game engine coursework last year
bool Encoder::ReadInData() {

	//clean out current stored data
	inputData.clear();

	//setup input stream
	ifstream f(inputFilepath.c_str(), ios::in);

	if (!f) {//Oh dear, it can't find the file :(
		cout << "Aw nuts. No file to read in." << endl;
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

	cout << "Read in data successfully!" << endl;
	return true;
}

bool Encoder::WriteOutData() {
	//setup input stream
	ofstream f(outputFilepath.c_str(), ios::out);

	if (!f) {
		cout << "Aw nuts. File output failed." << endl;
		return false;
	}

	//take characters from outputData and print to file
	uint dataSize = outputData.size();
	for (uint i = 0; i < dataSize; i++) {
		f << outputData[i];
	}

	return true;
}
