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
			if (bal != 0) {
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
			cout << cur->data << ' ';
			display(cur->next);
		}
		else {
			cout << endl;						// once the list has finished display, a newline character is printed
		}
	}

	void print(Node<string> *cur) {
		if (cur != NULL) {
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
			head = curHead->next;				// head is set to the next node and used at the end to call the next destroyDataList()
			delete curHead;						// deletes current node
			curHead = NULL;						// sets dangling pointer to null
			destroyDataList(head);				// loop back with next node.
		}
	}
};

class Process {
public:
	int duration, lifeTime, arrivalTime, id;

	Process() {
		duration = lifeTime = 1;		// set default constructor
		arrivalTime = 0;
		id = 1;
	}
	Process(int newId , string arrTime , string dur) {	// custom constructor sets properties to provided values
		duration = lifeTime = parseInt(dur);
		arrivalTime = parseInt(arrTime);
		id = newId;
	}

	int parseInt(string str) {			// returns int value of provided string
		int holder;
		stringstream ss(str);
		ss >> holder;
		return holder;
	}
	void displayProperties(int time, string state = "executing") {	// prints to console
		cout << setw(4) << time << " " << setw(9) << id << " " << setw(8) << duration << " " << state << endl;
	}
	void toFile(string fName , int time, string state = "executing") {	//prints to file
		ofstream fout(fName.c_str() , ios_base::app);
		fout << setw(4) << time << " " << setw(9) << id << " " << setw(8) << duration << " " << state << endl;
	}
};

void fifo(Process *arr[] , int &time, int size) {
	for (int i = 1; i <= size; i++) {
		arr[i]->displayProperties(time, "start");
		while (arr[i]->lifeTime >= 1) {
			if (arr[i]->lifeTime <= 1) {
				time++;
				arr[i]->displayProperties(time, "end");
				break;
			}
			time++;
			arr[i]->displayProperties(time);
			arr[i]->lifeTime--;
		}/**/
		//arr[i]->displayProperties(time, "end");
		delete[] arr[i];
		arr[i] = NULL;
	}
}

Process *shift(Process **backLog, int &backupCtr) {
	Process *first = backLog[1];

	for (int j = 1; j < backupCtr; j++) {
		backLog[j] = backLog[j + 1];
	}

	return first;
}

bool rebalance(Process **queue , Process **backLog , int &index , int &size, int &backupCtr) {
	//cout << "rebalance: " << queue[index]->id << " " << queue[index]->lifeTime << endl;
	delete[] queue[index];
	//cout << size << " " << backupCtr << endl;
	if (size > 1) {
		for (int i = index; i < size; i++) {
			queue[i] = queue[i + 1];
		}
	}
	if (backupCtr > 0) {
		queue[size] = shift(backLog, backupCtr);
		backupCtr--;
	}
	else {
		size--;
	}
	if (size > 0) {
		return true;
	}
	else {
		return false;
	}
}

void roundRobin(Process **queue, Process **backLog, int &time, int size, int quantum, int &index, int &backupCtr , int totalTime) {
	bool flag = true, proc = true;
	string fileOutput = "ps.txt";
	while(flag){
		proc = true;
		if (index > size) {
			index = 1;
		}
		if (queue[index]->lifeTime > 0) {
			queue[index]->displayProperties(time, "start");
			//queue[index]->displayProperties(time);
			for (int j = 0; j < quantum; j++) {
				if (queue[index]->lifeTime < 1) {
					queue[index]->displayProperties(time, "end");
					flag = rebalance(queue, backLog, index, size, backupCtr);
					proc = false;
					break;
				}
				queue[index]->lifeTime--;
				time++;
				queue[index]->displayProperties(time);
			}
		}
		else {
			queue[index]->displayProperties(time, "end");
			flag = rebalance(queue, backLog, index, size, backupCtr);
			proc = false;
		}
		if (proc) {
			index++;
		}
		//cout << index << " " << size << endl;
	}
	//queue[1]->displayProperties(time);
}

void buildArr(Process *arr[] , string str , int index) {
	string arrTime, dur;
	arrTime = str.substr(0, str.find(','));
	dur = str.substr(str.find(',') + 1);
	Process *proc = new Process(index, arrTime, dur);
	arr[index] = proc;
}

int main(int argc, char *argv[]) {
	if (!argv[1]) {
		exit(0);
	}
	string params = argv[1];
	string curParam;
	List<string> *paramList = new List<string>();
	int firstBreak = 0;
	while (firstBreak > -1) {
		firstBreak = params.find(';');
		curParam = params.substr(0, firstBreak);
		params = params.substr(firstBreak + 1);
		int valPos = curParam.find('=');
		string key = curParam.substr(0, valPos);
		string val = curParam.substr(valPos + 1);

		paramList->insertAtEnd(curParam);
	}
	//paramList->display(paramList->head);
	string fName, scheduling;
	int size, quantum = 1;
	Node<string> *cur = paramList->head;
	while (true) {
		if (cur == NULL) {
			break;
		}
		string keyStr = cur->data.substr(0, cur->data.find('='));
		if (keyStr == "input") {
			fName = cur->data.substr(cur->data.find('=')+1);
		}
		else if (keyStr == "size") {
			stringstream ss(cur->data.substr(cur->data.find('=') + 1));
			ss >> size;
		}
		else if (keyStr == "scheduling") {
			scheduling = cur->data.substr(cur->data.find('=') + 1);
		}
		else if (keyStr == "quantum") {
			stringstream ss(cur->data.substr(cur->data.find('=') + 1));
			ss >> quantum;
		}
		cur = cur->next;
	}
	Process **queue = new Process*[size + 1];
	Process **backLog;
	paramList->destroyDataList(paramList->head);
	
	fstream fin(fName.c_str());
	string txtLine;
	if (fin.peek() == EOF) {
		return 0;
	}
	string temp;
	int actualSize = 0, totalTime = 0, backupCtr = 0;
	List<string> *words = new List<string>();
	while (getline(fin, txtLine)) {
		if (txtLine[0] == '#') {
			continue;
		}

		stringstream fileLine(txtLine);
		txtLine.erase(remove(txtLine.begin(), txtLine.end(), '\r'), txtLine.end());
		if (actualSize < size) {
			actualSize++;
			buildArr(queue, txtLine, actualSize);
			totalTime += queue[actualSize]->duration;
		}
		else {
			words->insertAtEnd(txtLine);
			backupCtr++;
		}
	}/**/
	backLog = new Process*[backupCtr + 1];
	cur = words->head;
	int i = 1;
	while (cur != NULL) {
		buildArr(backLog, cur->data, i);
		backLog[i]->id = i + size;
		totalTime += backLog[i]->duration;
		i++;
		cur = cur->next;
	}
	string header = "time processId duration action";
	string foutName= "ps.txt";
	cout << header << endl;
	ofstream fout(foutName.c_str());
	fout << header << endl;
	int time = 0;
	int j = 1;
	if (actualSize < size) {
		size = actualSize;
	}
	if (scheduling == "fifo") {
		fifo(queue, time, size);
		while (j < backupCtr){
			int length = 0;
			for (int k = 0; k < size; k++) {
				queue[k%size + 1] = backLog[j];
				j++;
				length++;
				if (j > backupCtr) {
					break;
				}
			}
			fifo(queue, time, length);
		}
	}
	else {
		int index = 1;
		roundRobin(queue, backLog, time, size, quantum, index, backupCtr , totalTime);
	}
	return 0;
}