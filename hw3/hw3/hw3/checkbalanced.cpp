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

	void push(T newVal) {
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

	//void addNode(Node *cur, string prefix, string injection) {	// used to add a node after @prefix
	void addNode(Node<T> *cur, T prefix, T injection) {	// used to add a node after @prefix
		if (cur != NULL) {
			Node<T> *temp;
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

	void pop() {
		if (head != last) {
			Node<T> *temp = last;
			last = last->prev;
			last->next = NULL;
			delete temp;
			temp = NULL;
		}
		else {
			delete head;
			head = last = NULL;
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
			cout << cur->data;
			display(cur->next);
		}
		else {
			cout << endl;						// once the list has finished display, a newline character is printed
		}
	}

	T top() {
		if (head != NULL) {
			return last->data;
		}
		else {
			return "UNDEFINED";
		}
	}

	void destroyList(Node<T> *curHead) {		// called at the end of the program to delete allocated memory
		if (curHead != NULL) {
			head = curHead->next;				// head is set to the next node and used at the end to call the next destroyWordList()
			delete curHead;						// deletes current node
			curHead = NULL;						// sets dangling pointer to null
			destroyList(head);				// loop back with next node.
		}
	}
};

template <class T>
struct LineNode {					// same-ish structure as Node above
	T *start;
	int lineNumber;					// will be taken out sooni-ish and replaced with a better mechanism of counting lines
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

	LineList<T>() {											// general constructor
		head = NULL;
		last = NULL;
	}
	LineList<T>(T *wordHead) {							// constructor with pointer to List<T>
		LineNode<T> *temp;
		if (head == NULL) {								// if no items, add first
			temp->start = wordHead;
			head = temp;
			last = head;								// last points to head
			head->lineNumber = 1;
		}
		else {
			temp->start = wordHead;						// otherwise, add new List to end of LineList
			temp->prev = last;
			temp->lineNumber = last->lineNumber + 1;
			last->next = temp;
			last = temp;
		}
	}

	void insertAtEnd(T *wordHead) {				// similar magic/logic as method in List
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
	if (!argv[1]) {		// exit if no argument is provided
		exit(0);
	};

	string fName = argv[1];
	fName = fName.substr(fName.find('=')+1);		// grabs file name from argv[1]

	fstream fin(fName.c_str());
	string txtLine;
	List<string> *stack = new List<string>();
	if (!fin.good()) {
		return 0;
	} else if (fin.peek() == EOF) {
		return 0;
	}

	bool dbQ = false;
	bool snQ = false;
	int ctr = 0;
	bool hasErr = false;

	string data;
	while (getline(fin, txtLine)) {
			ctr++;				// line counter for display
		if (txtLine[0] == '#') {
			continue;
		}
		List<string> *words = new List<string>();
		stringstream fileLine(txtLine);
		char letter;
		int charCtr = 1;
		while (fileLine.get(letter)) {	//reads line char-by-char
			data = letter;				// char -> string
			if (letter != '\r') {		// check to not grab the line return character
				words->push(data);		//
				if (letter == ')') {															// lines 384 - 451 is a series of flow control statements to 
					if (stack->last != NULL) {													// determine whether a character gets pushed or popped
						if (stack->top() == "(") {												// compares top to see if it corresponds with closing token
							stack->pop();
						}
						else {
							cout << "Error at line " << ctr << ": top()=" << stack->top() << ";current=" << data << endl;
							hasErr = true;
						}
					}
					else {
						cout << "Error at line " << ctr << ": top()=" << stack->top() << ";current=" << data << endl;
						hasErr = true;
						//stack->push(data);
					}
				} else if (letter == ']') {
					if (stack->last != NULL) {
						if (stack->top() == "[") {
							stack->pop();
						}
						else {
							cout << "Error at line " << ctr << ": top()=" << stack->top() << ";current=" << data << endl;
							hasErr = true;
						}
					}
					else {
						cout << "Error at line " << ctr << ": top()=" << stack->top() << ";current=" << data << endl;
						hasErr = true;
						//stack->push(data);
					}
				} else if (letter == '}') {
					if(stack->last != NULL){
						if (stack->top() == "{") {
							stack->pop();
						}
						else {
							cout << "Error at line " << ctr << ": top()=" << stack->top() << ";current=" << data << endl;
							hasErr = true;
						}
					}
					else {
						cout << "Error at line " << ctr << ": top()=" << stack->top() << ";current=" << data << endl;
						hasErr = true;
						//stack->push(data);
					}
				} else if (letter == '"') {
					if(stack->last != NULL && dbQ){					// checks if the stack is empty and if a " has been pushed to the stack
						if (stack->top() == "\"") {					// top is a "
							stack->pop();							
							dbQ = false;							// sets double quotes flag to false
						}
						else {										// top is not a "; print error and set hasErr to true
							cout << "Error at line " << ctr << ": top()=" << stack->top() << ";current=" << data << endl;
							hasErr = true;
						}
					} else {										// otherwise, push and set dbQ to true for next encountered "
						stack->push("\"");
						dbQ = true;
					}
				} else if (letter == '\'') {					// checks if the stack is empty and if a ' has been pushed to the stack
					if (stack->last != NULL && snQ) {
						if (stack->top() == "'") {
							stack->pop();
							snQ = false;							// sets single quotes flag to false
						}
						else {										// top is not a '; print error and set hasErr to true
							cout << "Error at line " << ctr << ": top()=" << stack->top() << ";current=" << data << endl;
							hasErr = true;
						}
					} else {
						stack->push("'");
						snQ = true;							// sets single quotes flag to true
					}
				} else if (letter == '(' || letter == '[' || letter == '{') {//
					stack->push(data);
				}

			}
			charCtr++;
		}
		while(stack->top() == "\"" || stack->top() == "'"){
			stack->pop();
		}
	}
	if (stack->head == NULL) {// && !hasErr
		if (!hasErr) {
			cout << "Correct" << endl;
		}
	}
	else {
		cout << "Error at line " << ctr+1 << ": top()=" << stack->top() << ";current=$" << endl;
	}
	stack->destroyList(stack->head);
	return 0;
}