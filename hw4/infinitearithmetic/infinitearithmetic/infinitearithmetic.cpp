#include <iostream>
#include <fstream>
#include <sstream>
#include <locale>
#include <algorithm>
#include <string>
#include <cstdlib>
#include <iomanip>
#include <math.h>

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
	int cap;

	List() {
		head = last = NULL;
		cap = 1;
	}
	List(int newCap) {
		head = last = NULL;
		cap = newCap;
	}
	List(T newVal, int newCap) {
		Node<T> *temp = new Node<T>(newVal);
		temp->next = NULL;
		temp->prev = NULL;
		cap = newCap;
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
			head = last = temp;		// head is set to new node if list is empty
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

	T top() {
		if (head != NULL) {
			return last->data;
		}
		else {
			return "UNDEFINED";
		}
	}

	void recalibrate(Node<T> *cur, T maxCap, int bal = 0) {
		if (cur == NULL) {
			if (bal != 0) {
				Node<T> *tempNode = new Node<T>();
				tempNode->data = bal;
				tempNode->next = head;
				head->prev = tempNode;
				tempNode->prev = NULL;
				head = tempNode;
			}
			return;
		}
		int upper = pow(10, maxCap);
		int temp = 0;
		cur->data += bal;
		if (cur->data >= upper) {
			temp = cur->data / upper;
			cur->data = cur->data % upper;
		}
		recalibrate(cur->prev, maxCap, temp);
	}

	void recalibrateSub(Node<T> *cur, T maxCap, int bal = 0) {
		if (cur == NULL) {
			if (bal != 0){
				insertAtHead(bal);
			}
		}
		else {
			int upper = pow(10, maxCap);
			int temp = 0;
			if (cur != head) {
				if (cur->data < 0) {
					temp = cur->data;
					if (cur->prev != NULL) {
						cur->prev->data--;
					}
					cur->data = upper + temp;
				}
				else if (cur->data >= upper) {
					cur->data += bal;
					temp = cur->data / upper;
					cur->data = cur->data % upper;
				}
			}
			else {
				if (head->data == 0 && head != last) {
					deleteHead();
				}
				else {

				}
			}
			recalibrateSub(cur->prev, maxCap, temp);
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

	void deleteHead() {		// handles all the node pointer tomfoolery
		head = head->next;
		head->prev->next = NULL;
		head->prev = NULL;
		delete head->prev;
	}

	void display(Node<T> *cur) {					// prints out node's data to console then calls itself with the next node
		if (cur != NULL) {
			/*if (cur == head && cur->data == 0 && cur != last) {
				deleteHead();
				display(head);
			}
			else {*/
				if (cur == head) {
					cout << cur->data << ' ';
				}
				else {			
					cout << setfill('0') <<  setw(cap) << cur->data << ' ';
				}
				display(cur->next);
			//}
		}
		else {
			cout << endl;						// once the list has finished display, a newline character is printed
		}
	}
	
	void print(Node<string> *cur){
		if (cur != NULL){
			cout << cur->data << ' ';
			print(cur->next);
		}
		else {
			cout << endl;
		}
	}

	string toString(Node<T> *cur) {
		if (cur != NULL) {
			string temp = "";
			stringstream ss;
			ss << cur->data;
			temp = ss.str();
			int dataSize = temp.size();
			if (cur == head && cur->data == 0 && cur != last) {
				deleteHead();
				toString(head);
			}
			else {
				for (int i = 0; i < cap - dataSize; i++) {
					temp = "0" + temp;
				}
			}

			return temp + toString(cur->next);
		}
		else {
			return "";
		}
	}

	void destroyDataList(Node<T> *curHead) {		// called at the end of the program to delete allocated memory
		if (curHead != NULL) {
			head = curHead->next;				// head is set to the next node and used at the end to call the next destroyWordList()
			delete curHead;						// deletes current node
			curHead = NULL;						// sets dangling pointer to null
			destroyDataList(head);				// loop back with next node.
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

	LineList<T>() {											// general constructor
		head = NULL;
		last = NULL;
	}
	LineList<T>(T *wordHead) {							// constructor with pointer to List
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

	T top() {
		if (head != NULL) {
			return last->data;
		}
		else {
			return "UNDEFINED";
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

int parseInt(string str) {
	int holder;
	stringstream ss(str);
	ss >> holder;
	
	return holder;
}

bool isZero(Node<int> *node, bool flag = true) {
	if (node == NULL) {
		return true;
	} else if (node->prev == NULL) {
		if (node->data < 0) {
			return true;
		}
		else {
			return false;
		}
	} else {
		isZero(node->next, flag);
	}
}
void buildList(List<int> *list , string str, int digCap) {
	int strLen = str.size();
	if (strLen < 1) {
		return;
	}
	int curPos = strLen - digCap;
	if (curPos < 0) {
		curPos = 0;
	}
	int curVal;
	curVal = parseInt(str.substr(curPos, strLen));
	list->insertAtHead(curVal);
	str = str.substr(0, curPos);
	buildList(list, str, digCap);
}

void addLists(Node<int> *op1, Node<int> *op2, List<int> *result, int digCap) {
	int tempVal1, tempVal2, sum;
	tempVal1 = (op1 != NULL) ? op1->data : 0;
	tempVal2 = (op2 != NULL) ? op2->data : 0;
	sum = tempVal1 + tempVal2;

	result->insertAtHead(sum);
	if (op1 == NULL && op2 == NULL) {
		return;
	} else if (op1 == NULL) {
		addLists(op1, op2->prev, result, digCap);
	}
	else if (op2 == NULL) {
		addLists(op1->prev, op2 , result, digCap);
	}
	else {
		addLists(op1->prev, op2->prev, result, digCap);
	}
}

void subLists(Node<int> *op1, Node<int> *op2, List<int> *result, int digCap) {
	int tempVal1, tempVal2, diff;
	tempVal1 = (op1 != NULL) ? op1->data : 0;
	tempVal2 = (op2 != NULL) ? op2->data : 0;
	diff = tempVal1 - tempVal2;
	result->insertAtHead(diff);
	if (op1 == NULL && op2 == NULL) {
		return;
	}
	else if (op1 == NULL) {
		subLists(op1, op2->prev, result, digCap);
	}
	else if (op2 == NULL) {
		subLists(op1->prev, op2, result, digCap);
	}
	else {
		subLists(op1->prev, op2->prev, result, digCap);
	}
}

List<int> *multLists(Node<int> *op1, Node<int> *op2, List<int> *opList1, List<int> *opList2, List<int> *result, int digCap, int ctr , List<int> *holder = NULL) {
	List<int> *newRes = new List<int>(digCap);
	int prod;
	if (op2 == NULL) {
		return holder;
	}
	else if (op1 == NULL && op2 != NULL) {
		List<int> *tempList = new List<int>(digCap);
		result->recalibrate(result->last, digCap);
		if (holder != NULL) {
			for (int i = 0; i < ctr; i++) {
				result->insertAtEnd(0);
			}
			addLists(result->last, holder->last, newRes, digCap);
			newRes->recalibrate(result->last, digCap);
			result->destroyDataList(result->head);
			result = newRes;
		}
			multLists(opList1->last, op2->prev, opList1, opList2, tempList, digCap, ctr + 1, result); // resets top operand and moves bottom to prev node. multiply again
	}
	else if (op1 != NULL && op2 != NULL) {
		prod = op1->data * op2->data;
		result->insertAtHead(prod);
		multLists(op1->prev, op2, opList1, opList2, result, digCap, ctr , holder);
	}
}

int divLists(Node<int> *op1, Node<int> *op2, List<int> *result, int digCap, int ctr = 0) {
	subLists(op1, op2, result, digCap);
	result->recalibrateSub(result->last, digCap);
	bool flag = (result->head->data < 0);
	List<int> *temp = new List<int>(digCap);
	if (!flag) {
		divLists(result->last , op2 , temp , digCap , ctr+1);
	}
	else {
		return ctr;
	}
}

int main(int argc, char *argv[]) {
	if (!argv[1]) {
		exit(0);
	}
	argv[1] = &argv[1][6];

	string fName = argv[1];
	int split = fName.find(';');
	string digStr = fName.substr(split + 1);
	fName = fName.substr(0, split);
	split = digStr.find('=');
	digStr = digStr.substr(split + 1);
	int digPerNode;
	stringstream ss(digStr);
	ss >> digPerNode;
	fstream fin(fName.c_str());
	string txtLine;
	List<string> *opStack = new List<string>(); // used to hold the operators
	List<string> *postFixStack = new List<string>(); // used to hold the postfix expression
	if (fin.peek() == EOF) {
		return 0;
	}
	string temp;
	string operand1, operand2;
	int ctr = 1;
	bool isNeg = false;
	bool opFull = false;
	while (getline(fin, txtLine)) { // this loop build the stack
		List<string> *stackHolder = new List<string>();
		List<string> *words = new List<string>();

		stringstream fileLine(txtLine);
		string data;
		char letter;
		string opHolder = "";
		while (fileLine.get(letter)) {
			if (opHolder != "") {
				opHolder = opHolder.erase(0, min(opHolder.find_first_not_of('0'), opHolder.size() - 1));
			}
			data = letter;
			//opStack->print(opStack->head);
			if (letter == '(') {
				opStack->push(data);
				if (fileLine.peek() == '-') {
					isNeg = true;
				}
			}
			else if (letter == ')') {
				if (opFull) {
					postFixStack->push(opHolder);
					opHolder = "";
					opFull = false;
				}
				while (opStack->top() != "(") {		// keep popping from opStack and pushing to the postFixStack until open paren is found.
					postFixStack->push(opStack->top());
					opStack->pop();
				}
				opStack->pop();
			}
			if (letter == '*' || letter == '/'){
				if (opHolder != "") {
					postFixStack->push(opHolder);
					opHolder = "";
				}
					opStack->push(data);
			} else if (letter == '+'){
				if (opStack->top() == "*" || opStack->top() == "/"){
					postFixStack->push(opStack->top());
					opStack->pop();
					if (opStack->top() == "+" || opStack->top() == "-") {
						postFixStack->push(opStack->top());
						opStack->pop();
					}
				}
				else {
					if (opHolder != "") {
						postFixStack->push(opHolder);
						opHolder = "";
					}
				}
				opStack->push(data);
			}
			else if (letter == '-') {
				if (!isNeg) {
					if (opHolder != "") {
						postFixStack->push(opHolder);
						opHolder = "";
					}
					opStack->push(data);
				}
			}
			if (letter >= 48 && letter <= 57){
				if (isNeg) {
					opHolder = "-";
					isNeg = false;
				}
				opHolder += data;
				opFull = true;
			}
			else {
				opFull = false;
			}
			if (letter != '\r') {
				words->insertAtEnd(data);
			}
		}/**/
		if (opHolder != "") {
			postFixStack->push(opHolder);
			opHolder = "";
		}
		while (opStack->top() != "UNDEFINED") { // keeps popping and pushing until empty.
			postFixStack->push(opStack->top());
			opStack->pop();
		}
		//postFixStack->print(postFixStack->head);
		Node<string> *curNode = postFixStack->head; // used to traverse the postfixstack
		bool negate = false;
		while (curNode != NULL) {
			List<int> *opList1 = new List<int>(digPerNode);
			List<int> *opList2 = new List<int>(digPerNode);
			List<int> *result = new List<int>(digPerNode);
			//stackHolder->display(stackHolder->head);
			if (curNode->data != "+" && curNode->data != "-" && curNode->data != "*" && curNode->data != "/") {
				stackHolder->push(curNode->data); // if it's a number, push to the stackHolder that holds values
			}
			else { // otherwise evaluate the symbol, pop the top two numbers from the stack and evaluate the expression
				if (curNode->data == "*") { // *
					operand2 = stackHolder->top();
					stackHolder->pop();
					operand1 = stackHolder->top();
					stackHolder->pop();
					string temp;
					if (operand1.size() < operand2.size()) {
						temp = operand2;
						operand2 = operand1;
						operand1 = temp;
					}
				}
				else if (curNode->data == "/") { // /
					operand2 = stackHolder->top();
					stackHolder->pop();
					operand1 = stackHolder->top();
					stackHolder->pop();
				}
				else if (curNode->data == "+") { // +
					operand2 = stackHolder->top();
					stackHolder->pop();
					operand1 = stackHolder->top();
					stackHolder->pop();
				}
				else if (curNode->data == "-") { // -
					operand2 = stackHolder->top();
					stackHolder->pop();
					operand1 = stackHolder->top();
					stackHolder->pop();
				}
				/*
					START NEGATIVE VALUE MANIPULATION
				*/
				if (operand1[0] == '-' && operand2[0] != '-' && curNode->data == "+") {
					operand1.erase(remove(operand1.begin(), operand1.end(), '-'), operand1.end());
					operand2.erase(remove(operand2.begin(), operand2.end(), '-'), operand2.end());
					//std::cout << "hit " << operand1 << " : " << operand2 << endl;
					string tempOp = operand1;
					operand1 = operand2;
					operand2 = tempOp;
					curNode->data = "-";
					if (operand1.size() < operand2.size()) {
						temp = operand2;
						operand2 = operand1;
						operand1 = temp;
					}
				}
				else if (operand1[0] != '-' && operand2[0] == '-' && curNode->data == "+") {
					operand2.erase(remove(operand2.begin(), operand2.end(), '-'), operand2.end());
					string tempOp = operand1;
					operand1 = operand2;
					operand2 = tempOp;
				}
				else if (operand1[0] == '-' && operand2[0] == '-' && curNode->data == "+") {
					operand1.erase(remove(operand1.begin(), operand1.end(), '-'), operand1.end());
					operand2.erase(remove(operand2.begin(), operand2.end(), '-'), operand2.end());
					/*curNode->data = "-";*/
					negate = true;
				}
				else if (operand1[0] == '-' && operand2[0] == '-' && curNode->data == "-") {
					operand1.erase(remove(operand1.begin(), operand1.end(), '-'), operand1.end());
					operand2.erase(remove(operand2.begin(), operand2.end(), '-'), operand2.end());
					/*curNode->data = "+";*/
					//negate = true;
				}
				else if (operand1[0] == '-' && operand2[0] != '-' && (curNode->data == "/" || curNode->data == "*")) {
					operand2.erase(remove(operand2.begin(), operand2.end(), '-'), operand2.end());
					negate = true;
				}
				else if (operand1[0] != '-' && operand2[0] == '-' && (curNode->data == "/" || curNode->data == "*")) {
					operand1.erase(remove(operand1.begin(), operand1.end(), '-'), operand1.end());
				}
				else if (operand1[0] == '-' && operand2[0] != '-' && (curNode->data == "/" || curNode->data == "*")) {
					operand2.erase(remove(operand2.begin(), operand2.end(), '-'), operand2.end());
				}
				else if (operand1[0] == '-' && operand2[0] == '-' && (curNode->data == "/" || curNode->data == "*")) {
					operand1.erase(remove(operand1.begin(), operand1.end(), '-'), operand1.end());
					operand2.erase(remove(operand2.begin(), operand2.end(), '-'), operand2.end());
					negate = true;
				}
				/*
				END NEGATIVE VALUE MANIPULATION
				*/

				operand1.erase(remove(operand1.begin(), operand1.end(), ' '), operand1.end());
				operand2.erase(remove(operand2.begin(), operand2.end(), ' '), operand2.end());
				operand2.erase(remove(operand2.begin(), operand2.end(), '\r'), operand2.end());
				buildList(opList1, operand1, digPerNode);//converts string to int List<int>
				buildList(opList2, operand2, digPerNode);
				if (curNode->data == "*") {
					result = multLists(opList1->last, opList2->last, opList1, opList2, result, digPerNode, 0);
					result->recalibrate(result->last, digPerNode);//recalibrate readjusts the link lists to have no more than digitsPerNode numbers in each node
				}
				else if (curNode->data == "/") {
					int quotient = divLists(opList1->last, opList2->last, result, digPerNode);
					result->destroyDataList(result->head);
					result->insertAtHead(quotient);
					if (result->head->data == 0) {
						result->deleteHead();
					}
					result->recalibrate(result->last, digPerNode);//recalibrate readjusts the link lists to have no more than digitsPerNode numbers in each node
				}
				else if (curNode->data == "+") {
					addLists(opList1->last, opList2->last, result, digPerNode);
					if (result->head->data == 0) {
						result->deleteHead();
					}
					result->recalibrate(result->last, digPerNode);//recalibrate readjusts the link lists to have no more than digitsPerNode numbers in each node
				}
				else if (curNode->data == "-") {
					subLists(opList1->last, opList2->last, result, digPerNode);
					if (result->head->data == 0) {
						result->deleteHead();
					}
					result->recalibrateSub(result->last, digPerNode);//recalibrate readjusts the link lists to have no more than digitsPerNode numbers in each node
				}
				if (result->head->data == 0 && result->head != result->last) {
					result->deleteHead(); // if there is more than one element, and the top element is 0, it gets deleted
				}
				//result->display(result->head);

				string endStr = result->toString(result->head);
				endStr = endStr.erase(0, min(endStr.find_first_not_of('0'), endStr.size() - 1));
				if (negate) {
					endStr = "-" + endStr;
					negate = false;
				}
				stackHolder->push(endStr);
				ctr++;
			}
			//result->destroyDataList(result->head);
			curNode = curNode->next;
		}
		std::cout << txtLine << "=";
		stackHolder->print(stackHolder->head);
	}
	return 0;
}