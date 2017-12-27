#include <iostream>
#include <fstream>
#include <sstream>
#include <locale>
#include <algorithm>
#include <string>
#include <cstdlib>
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
		while(head->data == 0) {
			deleteHead();
		}
		if (cur == NULL) {
			if (bal != 0) {
				insertAtHead(bal);
			}
			return;
		}
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
			if (head->data == 0) {
				deleteHead();
			}
		}
		recalibrateSub(cur->prev, maxCap, temp);
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

	void display(Node<T> *cur) {					// prints out node's word to console then calls itself with the next node
		string holder = "";
		stringstream ss;
		if (cur != NULL) {
			/*if (cur == head && cur->data == 0 && cur != last) {
				deleteHead();
				display(head->next);
			}
			else {
				ss << cur->data;
				holder = ss.str();
				while(holder.size() < cap) {
					if (cur == head) {
						break;
					}
					holder = "0" + holder;
				}
				if (cur != head->next) {
					cout << holder << ' ';
				}
				else {*/
					cout << cur->data << ' ';
				//}
				display(cur->next);
			//}
		}
		else {
			cout << endl;						// once the list has finished display, a newline character is printed
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
	}
	if (op1 == NULL) {
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
	if (op1 == NULL) {
		subLists(op1, op2->prev, result, digCap);
	}
	else if (op2 == NULL) {
		subLists(op1->prev, op2, result, digCap);
	}
	else {
		subLists(op1->prev, op2->prev, result, digCap);
	}
}

List<int> *multLists(Node<int> *op1, Node<int> *op2, List<int> *opList1, List<int> *opList2, List<int> *result, int digCap, int ctr) {
	List<int> *tempList = new List<int>();
	List<int> *newRes = new List<int>();
	for (int i = 0; i < ctr; i++) {
		tempList->insertAtHead(0);
	}
	int tempVal1, tempVal2, prod;
	if (op2 == NULL) {
		return result;
	}
	else if (op1 == NULL && op2 != NULL) {
		cout << op2->data << endl;
		addLists(result->last, tempList->last, newRes, digCap);

		newRes->recalibrate(result->last, digCap);
		result->recalibrate(result->last, digCap);

		result->destroyDataList(result->head);
		result = newRes;
		multLists(opList1->last, op2->prev, opList1, opList2, result, digCap, ctr + 1);
	}
	else if (op1 != NULL && op2 != NULL) {
		prod = op1->data * op2->data;
		result->insertAtHead(prod);
		multLists(op1->prev, op2, opList1, opList2, result, digCap, ctr + 1);
	}
	/*tempVal1 = (op1 != NULL) ? op1->data : 1;
	tempVal2 = (op2 != NULL) ? op2->data : 1;

	prod = tempVal1 * tempVal2;					// won't work (i think)
	while (op2 != NULL) {
	while (op1 != NULL) {
	tempList->insertAtHead(op1->data * op2->data);
	op1 = op1->prev;
	}
	op2 = op2->prev;
	}
	cout << "Multiply start: " << endl;
	tempList->display(tempList->head);
	tempList->recalibrate(tempList->last, digCap);
	tempList->display(tempList->head);
	cout << "Multiply end: " << endl;*/
}

int divLists(Node<int> *op1, Node<int> *op2, List<int> *result, int digCap, int ctr = 0) {
	subLists(op1, op2, result, digCap);
	result->recalibrateSub(result->last, digCap);
	List<int> *temp = new List<int>();
	bool flag = (result->head->data < 0);
	if (!flag) {
		divLists(result->last , op2 , temp , digCap , ctr+1);
	}
	else {
		result->destroyDataList(result->head);
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
	LineList<List<string> > *lines = new LineList<List<string> >();
	LineList<string> *stack = new LineList<string>();
	LineList<List<int> > *postFixStack = new LineList<List<int> >();
	if (fin.peek() == EOF) {
		return 0;
	}
	string temp;
	string operand1, operand2;
	int opPos;
	bool add, sub, mult, div;
	add = sub = mult = div = false;
	int ctr = 1;
	while (getline(fin, txtLine)) {
		List<string> *words = new List<string>();
		List<int> *opList1 = new List<int>(digPerNode);
		List<int> *opList2 = new List<int>(digPerNode);
		List<int> *result = new List<int>(digPerNode);

		stringstream fileLine(txtLine);
		string data;
		char letter;
		int opMultPos = txtLine.find('*');
		int opDivPos = txtLine.find('/');
		int opAddPos = txtLine.find('+');
		int opSubPos = txtLine.find('-');
		if (opMultPos > -1) { // *
			operand1 = txtLine.substr(0, opMultPos);
			operand2 = txtLine.substr(opMultPos + 1);
			mult = true;
		}
		else if (opDivPos > -1) { // /
			operand1 = txtLine.substr(0, opDivPos);
			operand2 = txtLine.substr(opDivPos + 1);
			div = true;
		}
		else if (opAddPos > -1) { // +
			operand1 = txtLine.substr(0, opAddPos);
			operand2 = txtLine.substr(opAddPos + 1);
			add = true;
		}
		else if (opSubPos > -1) { // -
			operand1 = txtLine.substr(0, opSubPos);
			operand2 = txtLine.substr(opSubPos + 1);
			sub = true;
		}
		operand1.erase(remove(operand1.begin(), operand1.end(), ' '), operand1.end());
		operand2.erase(remove(operand2.begin(), operand2.end(), ' '), operand2.end());
		operand2.erase(remove(operand2.begin(), operand2.end(), '\r'), operand2.end());
		buildList(opList1, operand1, digPerNode);
		buildList(opList2, operand2, digPerNode);
		if (mult) {
			result = multLists(opList1->last, opList2->last, opList1 , opList2 , result, digPerNode , 0);
			result->recalibrate(result->last, digPerNode);
			//cout << "multiDisp" << endl;
			cout << ctr << ": ";
			result->display(result->head);
			//cout << "multiDisp end" << endl;
		}
		else if (div) {
			int quotient = divLists(opList1->last, opList2->last, result, digPerNode);
			result->destroyDataList(result->head);
			result->insertAtHead(quotient);
			result->recalibrate(result->last, digPerNode);
			cout << ctr << ": ";
			result->display(result->head);
		}
		else if (add) {
			addLists(opList1->last, opList2->last, result, digPerNode);
			result->recalibrate(result->last, digPerNode);
			cout << ctr << ": ";
			result->display(result->head);
		}
		else if (sub) {
			subLists(opList1->last, opList2->last, result, digPerNode);
			result->recalibrateSub(result->last, digPerNode);
			cout << ctr << ": ";
			result->display(result->head);
		}
		add = sub = mult = div = false;
		while (fileLine.get(letter)) {
			data = letter;
			if (letter != '\r') {
				words->insertAtEnd(data);
			}
		}
		/*opList1->display(opList1->head);
		opList1->destroyDataList(opList1->last);
		opList2->display(opList2->head);
		opList2->destroyDataList(opList2->last);*/
		lines->insertAtEnd(words);
		ctr++;
	}
	
	//lines->display(lines->head);
	//lines->destroyLines(lines->last);
	return 0;
}