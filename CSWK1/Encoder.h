#pragma once

#include <iostream>
#include <unordered_map>
#include <vector>
#include <fstream>
#include <string>
#include <bitset>

using namespace std;

const int INPUTRANGE = 4;
const int OUTPUTSIZE = 2;

const string defSett1 = "1100";
const string defSett2 = "0011";
const string defFilepath = "../inputFile.txt";

const bool xorOneRef = false;
const bool xorTwoRef = true;


class Encoder {
public:
	Encoder(string xorSett1 = defSett1, string xorSett2 = defSett2, string filepath = defFilepath);
	~Encoder();

	void EncoderCycle();	//cycle the encoder (run one cycle of encoding)
	void EncoderSetting(bool xorNum, string xorSett);	//change the xor gates inputs

private:
	void XorGate1();	//activates xor gate 1
	void XorGate2();	//activates xor gate 2
	
	void Initialise(); //clean out all registers and outputs
	void RegisterCycle();	//move the data from each register across and read in the next input bit
	bool ReadInData(string filepath);	//read in data from file


	bool registerArr[INPUTRANGE];	//the data stored in the registers (and the input bit)
	bool currentOutput[OUTPUTSIZE];	//the bits to output

	vector<bool> inputData;	//the actual data stream to input
	int inputPos = 0;		//the current position of the bit to put into the input bit( registerArr[0] )

	bool xor1Inputs[4];	//stores where xor gate 1 recieves input from
	bool xor2Inputs[4];	//stores where xor gate 2 recieves input from



	const static string defSett1;	//default input settings for xor gate 1
	const static string defSett2;	//default input settings for xor gate 2
	const static string defFilepath;
};


