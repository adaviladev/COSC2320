#include <cstdlib>
#include <iostream>
#include <fstream>
#include <string>

using namespace std;

int main(int argc, char *argv[]) {
	if (!argv[1]) {
		//cout << "No input detected.\nTerminating program." << endl;
		exit(0);
	}
	if (argv[1][0] != 'f' && argv[1][8] != '=') {
		cout << "Invalid input.\nTerminating program." << endl;
		exit(0);
	}
	argv[1] = &argv[1][9];	// switches the address to the start of the file name. 
							// This was put in after Cabrera mentioned that the 
							// script will be batch running the files with the following format:
							// ./count filename="input1.txt"
	// Declare needed variables
	char ch; // Holds the current character from the input file
	int ctrW = 0; // Keeps count of the number of words
	int ctrN = 0; // Keeps count of the number of numbers
	bool isNeeded = false; // Indicates whether the last character needs to be counted
	bool flagW = false; // Indicates whether the last valid character was a letter
	bool flagN = false; // Indicates whether the last valid character was a number
	ifstream fin(argv[1]); // Gets second command line argument
	if (fin.good()) {
		// If the file exists, the program runs
		//cout << "File \"" << argv[1] << "\" found. Beginning count." << endl;
	} else {
		// If the file does not exist, a message is show and the program terminates
		cout << "File \"" << argv[1]<< "\" not found. Exiting program." << endl;
		exit(0);
	}
	while (fin.get(ch)) { // Reads the file character-by-character until EOF
		if ((ch >= 'A' && ch <= 'Z') || (ch >= 'a' && ch <= 'z') || (ch >= '0' && ch <= '9')) { // Sets the range of desired values {A-Z, a-z, 0,9} 
																								// (I'm sure there's a better way to do this. I tried
																								// splitting the number and letter checks into an if-elseif-else
																								// check, but the count kept coming back incorrect.
																								// So, I finally decided to just split it into a Needed vs NotNeeded
																								// verification and further validate from there
			if ((ch >= '0' && ch <= '9')) { // Checks if it was a number
				if (flagN == false) { // checks whether a number has finished being read
					flagN = true;
				}
			} else {
				if (flagW == false) { // checks whether a wordhas finished being read
					flagW = true;
				}
			}
			//cout << ch << endl;
			//cout << "words=" << ctrW << " numbers=" << ctrN << endl; // displays the output
			isNeeded = true; // Used for final check at the end
		} else {
			if (flagW && isNeeded) {	// The code reaches here when the current character followed a letter.
				flagW = false;			// flagW gets set to true to show that a word is not currently being read
				ctrW++;					// ctrW gets incremented
			} else if (flagN && isNeeded) {	// The code reaches here when the current character followed a Number.
				flagN = false;					// flagN gets set to true to show that a number is not currently being read
				ctrN++;							// ctrN gets incremented
			}
			isNeeded = false; // indicates to the next loop that the previous character was not a letter or number.
		}
		/*cout << "stat check: " << endl;
		cout << "Char: " << ch << ' ' << "flagW: " << flagW << ' '  << "flagN: " << flagN << ' ' << "isNeeded: " << isNeeded << endl;*/
	}
	if ((ch >= 'A' && ch <= 'Z') || (ch >= 'a' && ch <= 'z') || (ch >= '0' && ch <= '9')) {
		if (flagW && isNeeded) {	// if flagW is not triggered and the character is needed,
			ctrW++;					// ctrW gets incremented
		}
		if (flagN && isNeeded) {	// if flagN is not triggered and the character is needed,
			ctrN++;						// ctrN gets incremented
		}
	}
	cout << "words=" << ctrW << " numbers=" << ctrN << endl; // displays the output
	//system("PAUSE");
	return 0;
}