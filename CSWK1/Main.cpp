#include <iostream>
#include "Encoder.h"

using namespace std;

int main() {

	Encoder encoderProg;

	int x = 0;
	bool exitValue = false;

	while (!exitValue) {
		//menu options!
		cout << endl << "Type the number of the instruction to execute:" << endl;
		cout << "0: Add new number to the tree" << endl;
		cout << "1: Print the tree" << endl;
		cout << "2: Terminate tree" << endl;
		cout << "3: Search tree" << endl;
		cout << "4: End program" << endl;

		cin >> x;


		switch (x) {
		case 0: //add a new number
			cout << endl << "Please type the number you wish to add" << endl;
			cin >> x;
			coolTree.insertInteger(x);
			break;

		case 1: //print tree
			cout << endl << "Printing tree: " << endl;
			coolTree.printTree();
			cout << endl << "//////////////" << endl;
			break;

		case 2: //terminate tree
			coolTree.terminateTree();
			break;

		case 3: //search tree
			cout << endl << "Please type the number you wish to search" << endl;
			cin >> x;
			if (coolTree.searchTree(x)) {
				cout << endl << x << " exists in tree!" << endl;
			}
			else {
				cout << endl << x << " does not exist in tree." << endl;
			}
			break;

		default: //end program
			exitValue = true;
			break;
		}

	}

	return 0;





}

