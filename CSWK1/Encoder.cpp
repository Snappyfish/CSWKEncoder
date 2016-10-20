#include "Encoder.h"

Encoder::Encoder(string xorSett1, string xorSett2, string filepath) {
	EncoderSetting(xorOneRef, xorSett1);
	EncoderSetting(xorTwoRef, xorSett1);

	ReadInData(filepath);



}

Encoder::~Encoder() {



}


void Encoder::EncoderCycle() {
	//process xor gates
	XorGate1();
	XorGate2();

	//send currentOutput to file


	//cycle registers and input bit
	RegisterCycle();


}

void Encoder::EncoderSetting(bool xorNum, string xorSett) {
	bool* affectArray;
	if (xorNum) {
		affectArray = xor1Inputs;
	}
	else {
		affectArray = xor2Inputs;
	}

	string bitSettings = std::bitset<4>(xorSett).to_string();
	for (int i = 0; i < INPUTRANGE; i++) {
		affectArray[i] = bitSettings.at((INPUTRANGE - 1) - i);
	}

}



void Encoder::XorGate1() {
	int result = 0;
	for (int i = 0; i < INPUTRANGE; i++) {
		if (xor1Inputs[i])
			result += registerArr[i];
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




void Encoder::Initialise() {
	//for loop clean out registerArr
	//for loop clean out currentOutput

	inputData.clear();
	inputPos = 0;

	//set xor1Inputs to default
	//set xor2Inputs to default

}

void Encoder::RegisterCycle() {
	//cycle registers
	for (int i = (INPUTRANGE - 1); i > 0; i--) {
		registerArr[i] = registerArr[i - 1];
	}

	//read input bit
	if (registerArr[inputPos] != inputData.end) {
		registerArr[0] = inputData[inputPos];
		inputPos++;
	}

}

//adapted from my game engine coursework last year
bool Encoder::ReadInData(string filepath) {
	//clean out current stored data
	inputData.clear();

	//setup input stream
	ifstream f(filepath.c_str(), ios::in);

	if (!f) {//Oh dear, it can't find the file :(
		cout << "Aw nuts." << endl << endl;
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

	cout << "Read in data successfully!" << endl << endl;
	return true;
}