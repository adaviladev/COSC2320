#include <iostream>
#include <fstream>
#include <sstream>
#include <locale>
#include <algorithm>
#include <string>
#include <cstdlib>

using namespace std;

template <class T>
struct Node {		// Used for generating the list of words in a line				
	T data;	// Similar structure as the one used for lists. I'll probably try to just templatize it for future homeworks
	Node *next;
	Node *prev;

	Node() {						//general constructor
		next = prev = NULL;
	}
	Node(T newVal) {			//parametered constructor
		data = newVal;
		next = prev = NULL;
	}
};

template <class T>
class List {
public:
	Node<T> *head, *last;			// used to point to the first and last words in the current list

	List() {
		head = NULL;
		last = NULL;
	}
	List(T newVal) {
		Node<T> *temp = new Node<T>(newVal);
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

	void insertAtHead(T newVal) {
		Node<T> *temp = new Node<T>(newVal);
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

	void insertAfterHead(T newVal) {	// used to add a node immediately after the head
		Node<T> *temp = new Node<T>(newVal);		// Not used in this program
		temp->next = head->next;
		temp->prev = head;
		head->next->prev = temp;
		head->next = temp;
	}

	void insertAtEnd(T newVal) {
		if (head == NULL) {					// list is empty
			head = new Node<T>(newVal);
			last = head;					// head is made into a new node and last is made equivalent to it
			return;							// exits function
		}
		Node<T> *temp = new Node<T>(newVal);
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

	void addNode(Node<T> *cur, T prefix, T injection) {	// used to add a node after @prefix
		if (cur != NULL) {
			Node<T> * temp;

			if (prefix == "") {											// used to match the </injection> case of the insertword command
				insertAtHead(injection);
			}
			else if (cur->data == prefix) {						// if a word is matched with prefix, then 
				temp = new Node<T>(injection);							// the new node is added after			
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

	void deleteNode(Node<T> *cur, T str) {		// handles all the node pointer tomfoolery
		if (cur->data == str) {
			if (cur == head) {						// checks if the matched node is the head
				cur->next->prev = NULL;
				head = cur->next;
			}
			else if (cur == last) {				// checks if the matched node is the last
				cur->prev->next = NULL;
				last = cur->prev;
			}
			else {
				cur->prev->next = cur->next;		// adjusts prev and next pointers
				cur->next->prev = cur->prev;
			}
			delete cur;								// delete allocated pointer
			cur = NULL;								// set dangling pointer to NULL;
		}
		else {
			if (cur->next != NULL) {
				deleteNode(cur->next, str);			// current node doesn't match. Loop back with the next node
			}
		}
	}

	void display(Node<T> *cur) {					// prints out node's word to console then calls itself with the next node
		if (cur != NULL) {
			cout << cur->data << ' ';
			display(cur->next);
		}
		else {
			cout << endl;						// once the list has finished display, a newline character is printed
		}
	}

	void destroyWordList(Node<T> *curHead) {		// called at the end of the program to delete allocated memory
		if (curHead != NULL) {
			head = curHead->next;				// head is set to the next node and used at the end to call the next destroyWordList()
			delete curHead;						// deletes current node
			curHead = NULL;						// sets dangling pointer to null
			destroyWordList(head);				// loop back with next node.
		}
	}
};

template <class T>
struct LineNode {					// same-ish structure as Node above
	T *start;
	int lineNumber;
	LineNode *next, *prev;

	LineNode() {
		lineNumber = 1;
		start = new T();
		next = prev = NULL;
	}
	LineNode(T *newStart) {
		start = newStart;
		next = prev = NULL;
	}
};

template <class T>
class LineList {
public:
	LineNode<T> *head, *last;

	LineList() {											// general constructor
		head = NULL;
		last = NULL;
	}
	LineList(T *wordHead) {							// constructor with pointer to List
		LineNode<T> *temp;
		if (head == NULL) {
			temp->start = wordHead;
			head = temp;
			last = head;
			head->lineNumber = 1;
		}
		else {
			temp->start = wordHead;
			temp->prev = last;
			temp->lineNumber = last->lineNumber + 1;
			last->next = temp;
			last = temp;
		}
	}

	void insertAtEnd(T *wordHead) {						// similar magic/logic as method in List
		LineNode<T> *temp = new LineNode<T>();
		if (head == NULL) {								// checks if the list has any lines
			temp->start = wordHead;
			temp->lineNumber = 1;						// linenumber gets set to one, 
			temp->next = NULL;
			temp->prev = NULL;
			head = last = temp;
		}
		else {
			temp->start = wordHead;						// otherwise, the line is added to the end of the list
			temp->lineNumber = last->lineNumber + 1;
			temp->prev = last;
			temp->next = NULL;
			last->next = temp;
			last = temp;								// last gets reassigned
		}
	}
	void insertLineAt(LineNode<T> *cur, int num) {
		if (cur->lineNumber == num - 1) {				// checks for correct previous line number
			LineNode<T> *temp = new LineNode<T>();
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

	void insertWordAtLine(LineNode<T> *cur, int num, string pre, string target) {
		if (cur != NULL) {
			if (cur->lineNumber == num) {
				if (cur->start->head == NULL) {
					cur->start->insertAtHead(target);
				}
				else {
					cur->start->addNode(cur->start->head, pre, target);
				}
			}
			else {
				insertWordAtLine(cur->next, num, pre, target);
			}
		}
	}

	void deleteLineAt(LineNode<T> *cur, int num) {
		if (cur->lineNumber == num) {
			LineNode<T> *temp = new LineNode<T>();
			if (num == 1) {
				head = cur->next;
				head->prev = NULL;
			}
			else {
				temp = cur->prev;
				temp->next = cur->next;
				cur->next->prev = temp;
			}
			if (cur == last) {
				last = cur->prev;
				last->next = NULL;
			}
			decrementLineNumber(cur);
			delete cur;
			cur = NULL;
		}
		else {
			if (cur->next != NULL) {
				deleteLineAt(cur->next, num);
			}
		}
	}

	void deleteWordAtLine(LineNode<T> *cur, int num, string str) {
		if (cur != NULL) {
			if (cur->lineNumber == num) {
				cur->start->deleteNode(cur->start->head, str);
			}
			else {
				deleteWordAtLine(cur->next, num, str);
			}
		}
	}

	void incrementLineNumber(LineNode<T> *cur) {
		if (cur != NULL) {
			cur->lineNumber = cur->lineNumber + 1;
			incrementLineNumber(cur->next);
		}
	}

	void decrementLineNumber(LineNode<T> *cur) {
		if (cur != NULL) {
			cur->lineNumber = cur->lineNumber - 1;
			decrementLineNumber(cur->next);
		}
	}
	void display(LineNode<T> *cur) {
		if (cur != NULL) {
			cur->start->display(cur->start->head);
			if (cur->next != NULL) {
				display(cur->next);
			}
		}
	}

	void destroyLines(LineNode<T> *curHead) {
		if (curHead != NULL) {
			head = curHead->next;
			curHead->start->destroyWordList(curHead->start->head);
			delete curHead;
			curHead = NULL;
		}
	}
};

int main(int argc, char *argv[]) {

	if (!argv[1]) {
		exit(0);
	}
	string input = argv[1];
	int pos = input.find(';');
	string fileNameHolder = input.substr(0, pos);
	string fileActionHolder = input.substr(pos);
	pos = fileNameHolder.find('=');
	string fName = fileNameHolder.substr(pos+1);
	pos = fileActionHolder.find('=');
	string fileActName = fileActionHolder.substr(pos+1);

	fstream fin(fName.c_str());
	fstream fActIn(fileActName.c_str());
	string commLine;
	string txtLine;
	LineList<List<string> > *lines = new LineList<List<string> >();
	if (fin.peek() == EOF) {
		return 0;
	}
	while (getline(fin, txtLine)) {
		List<string> *words = new List<string>();
		stringstream fileLine(txtLine);
		string word;
		while (fileLine >> word) {
			words->insertAtEnd(word);
		}
		lines->insertAtEnd(words);
	}
	while (getline(fActIn, commLine)) {
		locale loc;
		int keyValSplit = commLine.find('=');
		string command = commLine.substr(0, keyValSplit);
		string subCommand = commLine.substr(keyValSplit + 1);
		stringstream ss(subCommand);
		int argPos = subCommand.find(':');
		int lineNumber;
		string arguments;
		string target;
		string preFix;
		if (argPos == -1) {
			ss >> lineNumber;
		}
		else {
			arguments = subCommand.substr(argPos + 1);
			string lineStr = subCommand.substr(0, argPos);
			int targPos = arguments.find('/');
			if (targPos != -1) {
				target = arguments.substr(targPos + 1);
				target.erase(remove(target.begin(), target.end(), '\r'), target.end());
				preFix = arguments.substr(0, targPos);
			}
			else {
				target = arguments;
				target.erase(remove(target.begin(), target.end(), '\r'), target.end());
			}
			ss << lineStr;
			ss >> lineNumber;
		}
		if (command == "deleteline") {
			lines->deleteLineAt(lines->head , lineNumber);
		} else if (command == "insertline") {
			lines->insertLineAt(lines->head, lineNumber);
		} else if (command == "deleteword") {
			lines->deleteWordAtLine(lines->head, lineNumber, target);
		} else if (command == "insertword") {
			lines->insertWordAtLine(lines->head , lineNumber , preFix, target);
		}
	}
	lines->display(lines->head);
	lines->destroyLines(lines->last);
	return 0;
}