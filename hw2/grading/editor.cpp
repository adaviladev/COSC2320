#include <iostream>
#include <fstream>
#include <sstream>
#include <locale>
#include <algorithm>
#include <string>
#include <cstdlib>

using namespace std;

struct Node {		// Used for generating the list of words in a line				
	string word;	// Similar structure as the one used for lists. I'll probably try to just templatize it for future homeworks
	Node *next;
	Node *prev;

	Node() {						//general constructor
		next = prev = NULL;
	}
	Node(string newVal) {			//parametered constructor
		word = newVal;
		next = prev = NULL;
	}
};
class WordList {
public:
	Node *head, *last;			// used to point to the first and last words in the current list

	WordList() {
		head = NULL;
		last = NULL;
	}
	WordList(string newVal) {
		Node *temp = new Node(newVal);
		temp->next = NULL;
		temp->prev = NULL;
		if (head == NULL) {			// check to see if the first node exists
			head = last = temp;		// if not, head and last are set to the new node
		}
		else {
			temp->prev = head;		// otherwise, it's added to the end and last is reassigned to new node
			last->next = temp;
			last = temp;
		}
	}

	void insertAtHead(string newVal) {
		Node *temp = new Node(newVal);
		if (head == NULL) {
			head = temp;		// head is set to new node if list is empty
		}
		else {
			temp->next = head;	// otherwise the new node is moved to the beginning of 
			temp->prev = NULL;	// the list and head is reassigned to point to the new node
			head->prev = temp;
		}
		head = temp;
	}

	void insertAfterHead(string newVal) {	// used to add a node immediately after the head
		Node *temp = new Node(newVal);		// Not used in this program
		temp->next = head->next;
		temp->prev = head;
		head->next->prev = temp;
		head->next = temp;
	}

	void insertAtEnd(string newVal) {
		if (head == NULL) {					// list is empty
			head = new Node(newVal);
			last = head;					// head is made into a new node and last is made equivalent to it
			return;							// exits function
		}
		Node *temp = new Node(newVal);
		temp->prev = last;
		temp->next = NULL;
		if (head->next == NULL) {			// checks to see if there is only one item in the list
			head->next = temp;				// Not necessary now that I think about it, but it's already finished...
			last = temp;					// last gets reassigned to the new node
		}
		else {
			last->next = temp;
			last = temp;					// last gets reassigned to the new node
		}
	}

	void addNode(Node *cur , string prefix , string injection) {	// used to add a node after @prefix
		if (cur != NULL) {											
			Node * temp;
			if (prefix == "") {										// used to match the </injection> case of the insertword command
				insertAtHead(injection);
			} else if (cur->word == prefix) {						// if a word is matched with prefix, then 
				temp = new Node(injection);							// the new node is added after			
				if (cur->next != NULL) {							// used to check if the current node is the last node // Should have just compared it with last...
					temp->next = cur->next;
					temp->prev = cur;
					cur->next->prev = temp;
					cur->next = temp;
				}
				else {												// otherwise, the current node is the last node.
					temp->next = NULL;
					cur->next = temp;
					last = temp;									// last is reassigned
				}
			}
			else {
				addNode(cur->next, prefix, injection);				// no match found. loop back.
			}
		}
	}

	void deleteNode(Node *cur, string str) {		// handles all the node pointer tomfoolery
		if (cur->word == str) {						
			if (cur == head) {						// checks if the matched node is the head
				cur->next->prev = NULL;
				head = cur->next;
			} else if (cur == last) {				// checks if the matched node is the last
				cur->prev->next = NULL;
				last = cur->prev;
			} else {
				cur->prev->next = cur->next;		// adjusts prev and next pointers
				cur->next->prev = cur->prev;
			}
			delete cur;								// delete allocated pointer
			cur = NULL;								// set dangling pointer to NULL;
		} else {
			if (cur->next != NULL) {
				deleteNode(cur->next, str);			// current node doesn't match. Loop back with the next node
			}
		}
	}

	void display(Node *cur) {					// prints out node's word to console then calls itself with the next node
		if (cur != NULL) {
			cout << cur->word << ' ';
			display(cur->next);
		}
		else {
			cout << endl;						// once the list has finished display, a newline character is printed
		}
	}

	void destroyWordList(Node *curHead) {		// called at the end of the program to delete allocated memory
		if (curHead != NULL) {
			head = curHead->next;				// head is set to the next node and used at the end to call the next destroyWordList()
			delete curHead;						// deletes current node
			curHead = NULL;						// sets dangling pointer to null
			destroyWordList(head);				// loop back with next node.
		}
	}
};

struct LineNode {					// same-ish structure as Node above
	WordList *start;
	int lineNumber;
	LineNode *next, *prev;

	LineNode() {
		lineNumber = 1;
		start = new WordList();
		next = prev = NULL;
	}
	LineNode(WordList *newStart) {
		start = newStart;
		next = prev = NULL;
	}
};
class LineList {
public:
	LineNode *head, *last;

	LineList() {											// general constructor
		head = NULL; 
		last = NULL;
	}
	LineList(WordList *wordHead) {							// constructor with pointer to WordList
		LineNode *temp;
		if (head == NULL) {
			temp->start = wordHead;
			head = temp;
			last = head;
			head->lineNumber = 1;
		}
		else {
			temp->start = wordHead;
			temp->prev = last;
			temp->lineNumber = last->lineNumber+1;
			last->next = temp;
			last = temp;
		}
	}

	void insertAtEnd(WordList *wordHead) {				// similar magic/logic as method in WordList
		LineNode *temp = new LineNode();
		if (head == NULL) {								// checks if the list has any lines
			temp->start = wordHead;
			temp->lineNumber = 1;						// linenumber gets set to one, 
			temp->next = NULL;
			temp->prev = NULL;
			head = last = temp;
		}
		else {
			temp->start = wordHead;						// otherwise, the line is added to the end of the list
			temp->lineNumber = last->lineNumber+1;
			temp->prev = last;
			temp->next = NULL;
			last->next = temp;
			last = temp;								// last gets reassigned
		}
	}
	void insertLineAt(LineNode *cur , int num) {
		if (cur->lineNumber == num-1) {				// checks for correct previous line number
			LineNode *temp = new LineNode();
			temp->lineNumber = num;
			if (cur->next != NULL) {				// if not last 
				temp->next = cur->next;
				temp->prev = cur;
				cur->next->prev = temp;
				cur->next = temp;
				incrementLineNumber(temp->next);	// once line has been added, all next lines' lineNumber needs to get incremented
			}
			else {
				cur->next = temp;					// enters here when last LineNode matches check
				temp->next = NULL;
				temp->prev = cur;
				last = temp;
			}
		}
		else {
			if (cur->next != NULL) {
				insertLineAt(cur->next, num);		// otherwise, loop again with the next line
			}
		}
	}

	void insertWordAtLine(LineNode *cur, int num, string pre , string target) {		// inserts new WordList object at the current
		if (cur != NULL) {															// position if a match is found on lineNumber
			if (cur->lineNumber == num) {
				if (cur->start->head == NULL) {										// if the match is the head, then it just calls
					cur->start->insertAtHead(target);								// insertAtHead()
				} else {
					cur->start->addNode(cur->start->head, pre, target);				// otherwise it inserts with addNode()
				}
			}
			else {
				insertWordAtLine(cur->next, num, pre , target);						// no match on current? loop back with next LineNode
			}
		}
	}

	void deleteLineAt(LineNode *cur, int num) {			// inverse of insertWordAtLine()
		if(cur != NULL){
			if (cur->lineNumber == num) {				// match found
				LineNode *temp = new LineNode();
				if (num == 1) {							// if match is first line; move head to next node
					head = cur->next;					// and set prev pointer to NULL
					head->prev = NULL;
				}
				else {
					temp = cur->prev;					// reassigns pointers of next and prev
					temp->next = cur->next;
					cur->next->prev = temp;
				}
				if (cur == last) {						// if cur is last, last gets reassigned back one
					last = cur->prev;
					last->next = NULL;
				}
				decrementLineNumber(cur);				// decrement lineNumber of all next nodes
				delete cur;								// delete pointer
				cur = NULL;								// set dangling pointer to NULL
			}
			else {
				if (cur->next != NULL) {
					deleteLineAt(cur->next, num);		// no match? You know the drill
				}
			}
		}
	}

	void deleteWordAtLine(LineNode *cur, int num, string str) {
		if (cur != NULL) {
			if (cur->lineNumber == num) {
				cur->start->deleteNode(cur->start->head , str);		// match found. Call deleteNode() method of WordList(start)
			}
			else {
				deleteWordAtLine(cur->next, num, str);				// not a match: repeat with the next
			}
		}
	}

	void incrementLineNumber(LineNode *cur) {			// moves through each subsequent node and increments the line number
		if (cur != NULL) {
			cur->lineNumber = cur->lineNumber+1;
			incrementLineNumber(cur->next);
		}
	}

	void decrementLineNumber(LineNode *cur) {			// moves through each subsequent node and decrements the line number
		if (cur != NULL) {
			cur->lineNumber = cur->lineNumber-1;
			decrementLineNumber(cur->next);
		}
	}
	void display(LineNode *cur) {						// moves through each node
		if (cur != NULL) {
			cur->start->display(cur->start->head);		// calls display method of WordList property of LineNode
			if (cur->next != NULL) {
				display(cur->next);						// calls display with next LineNode object
			}
		}
	}

	void destroyLines(LineNode *curHead) {								// called at end of the program to free all heap memory
		if (curHead != NULL) {
			curHead->start->destroyWordList(curHead->start->head);		// runs through WordList and deletes each Node
			head = curHead->next;										// moves head to next LineNode
			delete curHead;												// delete current LineNode
			curHead = NULL;												// set dangling pointer to NULL
			destroyLines(head);											// loop back with new head
		}
	}
};

int main(int argc, char *argv[]) {

	if (!argv[1]) {											// no command-line argument? Exit.
		exit(0);
	}
	string input = argv[1];									// START FILE NAME PARSING
	int pos = input.find(';');								// 
	string fileNameHolder = input.substr(0, pos);			// this block parses the file names
	string fileActionHolder = input.substr(pos);			// out of the commandline input
	pos = fileNameHolder.find('=');							// using the specified delimiters
	string fName = fileNameHolder.substr(pos+1);			// 
	pos = fileActionHolder.find('=');						// 
	string fileActName = fileActionHolder.substr(pos+1);	// END FILE NAME PARSING

	fstream fin(fName.c_str());								// create input file fstream object
	fstream fActIn(fileActName.c_str());					// create command input fstream object
	string commLine;										// 
	string txtLine;											// 
	LineList *lines = new LineList();
	if (fin.peek() == EOF && fActIn.peek() == EOF) {		// if both files are empty, the program ends
		return 0;
	}
	while (getline(fin, txtLine)) {
		WordList *words = new WordList();
		stringstream fileLine(txtLine);
		string word;
		while (fileLine >> word) {							// reads words
			words->insertAtEnd(word);						// adds words to WordList words
		}
		lines->insertAtEnd(words);							// adds current WordList to LineList lines
	}
	while (getline(fActIn, commLine)) {
		locale loc;
		int keyValSplit = commLine.find('=');					// START COMMAND INPUT PARSE
		string command = commLine.substr(0, keyValSplit);		// used for flow control
		string subCommand = commLine.substr(keyValSplit + 1);	// gets rest of the line
		stringstream ss(subCommand);							// 
		int argPos = subCommand.find(':');						// used to determine if the rest of the line only has a number
		int lineNumber;											// END COMMAND INPUT PARSE
		string arguments;
		string target;
		string preFix;
		if (argPos == -1) {
			ss >> lineNumber;									// converts line number to int (lineNumber)
		}
		else {
			arguments = subCommand.substr(argPos + 1);
			string lineStr = subCommand.substr(0, argPos);
			int targPos = arguments.find('/');
			if (targPos != -1) {															// START VARIABLE ASSIGNMENT
				target = arguments.substr(targPos + 1);										// 
				target.erase(remove(target.begin(), target.end(), '\r'), target.end());		// -used to strip away \r from new word
				preFix = arguments.substr(0, targPos);										// 
			}																				// assignment based on presence 
			else {																			// of delimiters
				target = arguments;															// 
				target.erase(remove(target.begin(), target.end(), '\r'), target.end());		// -used to strip away \r from new word
			}																				// END VARIABLE ASSIGNMENT
			ss << lineStr;
			ss >> lineNumber;
		}
		if (command == "deleteline") {												// START FLOW CONTROL BLOCK
			lines->deleteLineAt(lines->head , lineNumber);							// 
		} else if (command == "insertline") {										// 
			lines->insertLineAt(lines->head, lineNumber);							// no explanation necessary. 
		} else if (command == "deleteword") {										// You know what this is doing.
			lines->deleteWordAtLine(lines->head, lineNumber, target);				// 
		} else if (command == "insertword") {										// 
			lines->insertWordAtLine(lines->head , lineNumber , preFix, target);		// END  FLOW CONTROL BLOCK
		}
	}
	lines->display(lines->head);													// calls method to displays the lines
	lines->destroyLines(lines->last);												// calls method to free all allocated memory
	return 0;
}