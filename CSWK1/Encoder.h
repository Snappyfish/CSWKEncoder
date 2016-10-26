#pragma once

#include <iostream>
#include <vector>
#include <fstream>
#include <string>
#include <bitset>

using namespace std;

const int INPUTRANGE = 4;
const int MAXPERMU = INPUTRANGE * INPUTRANGE;
const int OUTPUTSIZE = 2;

const bool XOR1REF = false;
const bool XOR2REF = true;

const string OUTPUTDIR = ".\\output\\";
const string FILEEXT = ".txt";


typedef unsigned int uint;


class Encoder {
public:
	Encoder(string xorSett1 = defSett1, string xorSett2 = defSett2, string inFilepath = defInFilepath, string outFilepath = defOutFilepath);
	~Encoder();

	bool RunEncoderPreStep();	//read in data
	bool RunEncoder(bool suppMsg = false);	//run the encoder!
	void RunEncoderFullCycle();	//run the encoder on every possible permutation

	void EncoderSetting(bool xorNum, string xorSett);	//change the xor gates inputs

	void SetInputPath(string path);	//change where the data will be input from
	void SetOutputPath(string path);	//change where the data will be output to

	bool EncoderCompare(string filepath1, string filepath2);	//compare two encoded files

private:
	void EncoderCycle();	//cycle the encoder (run one cycle of encoding)

	void XorGate1();	//activates xor gate 1
	void XorGate2();	//activates xor gate 2
	
	void InitialiseVars();		//clean out all registers and outputs
	void RegisterCycle();	//move the data from each register across and read in the next input bit
	bool ReadInData();	//read in data from file
	bool WriteOutData();	//print out currentOutput to file

	bool registerArr[INPUTRANGE];	//the data stored in the registers (and the input bit)
	bool currentOutput[OUTPUTSIZE];	//the bits to output

	vector<bool> inputData;		//the actual data stream to input
	vector<bool> outputData;	//the binary data to output
	uint inputPos = 0;			//the current position of the bit to put into the input bit( registerArr[0] )

	bool xor1Inputs[INPUTRANGE];		//stores where xor gate 1 recieves input from
	bool xor2Inputs[INPUTRANGE];		//stores where xor gate 2 recieves input from

	string inputFilepath;	//the path to read in from
	string outputFilepath;	//the path to output data to

	vector<vector<string>> duplEncoders;

	const static string defSett1;		//default input settings for xor gate 1
	const static string defSett2;		//default input settings for xor gate 2
	const static string defInFilepath;	//default input file location
	const static string defOutFilepath;	//default input file location
	const static string defDuplFilepath;
};


