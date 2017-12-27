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

	void insertSort(Node<T> *cur, T val) {	// builds a list and sorts it
		if (head == NULL) {					// used for building a sorted array later
			insertAtEnd(val);
		} else if (cur != NULL) {
			Node<T> *temp = new Node<T>(val);
			if (cur->data[0] < val[0]) {
				if (cur->next != NULL) {
					if (cur->next->data[0] < val[0]) {
						insertSort(cur->next, val);			// new node is still greater than all in current list
					}
					else {									// spot found. insert between current and next
						temp->next = cur->next;
						temp->prev = cur;
						temp->next->prev = temp;
						cur->next = temp;
					}
				}
				else {
					insertAtEnd(val); // Reached end of list. Insert at end.
				}
			}
			else {
				if (cur == head) {
					insertAtHead(val); // Less than head. Insert at Head
				}
				else {							// fallback used for inserting when less than the current
					temp->prev = cur->prev;
					temp->next = cur;
					temp->prev->next = temp;
					cur->prev = temp;
				}
			}
		}
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
	bool init;

	Process() {
		duration = lifeTime = 1;		// set default constructor
		arrivalTime = 0;
		id = 1;
		init = false;
	}
	Process(int newId , string arrTime , string dur) {	// custom constructor sets properties to provided values
		duration = lifeTime = parseInt(dur);
		arrivalTime = parseInt(arrTime);
		id = newId;
		init = false;
	}

	int parseInt(string str) {			// returns int value of provided string
		int holder;
		stringstream ss(str);
		ss >> holder;
		return holder;
	}
	void displayProperties(int time, string state = "executing") {	// prints to console
		cout << setw(4) << time << " " << setw(9) << id << " " << setw(8) << duration << " " << state <<endl;// " " << lifeTime << 
	}
	void toFile(string fName , int time, string state = "executing") {	//prints to file
		ofstream fout(fName.c_str() , ios_base::app);
		fout << setw(4) << time << " " << setw(9) << id << " " << setw(8) << duration << " " << state << endl;
	}
};

void fifo(Process *arr[] , int &time, int size) {
	for (int i = 1; i <= size; i++) {
		arr[i]->displayProperties(time, "start");
		while (arr[i]->lifeTime >= 1) {					// counts until the current process is done
			if (arr[i]->lifeTime <= 1) {				// end reached. Break out of while and go on to the next
				time++;
				arr[i]->displayProperties(time, "end");
				break;
			}
			time++;
			arr[i]->displayProperties(time);
			arr[i]->lifeTime--;
		}/**/
		//arr[i]->displayProperties(time, "end");
		delete[] arr[i];			// free memory
		arr[i] = NULL;				// clear dangling pointer
	}
}

Process *shift(Process **backLog, int &backupCtr) {
	Process *first = backLog[1];				// store current first item in backlog to be returned at the end

	for (int j = 1; j < backupCtr; j++) {
		backLog[j] = backLog[j + 1];			// reassign current index to contents of next index
	}

	return first;
}

void rebalance(Process **queue , Process **backLog , int &index , int &size, int &backupCtr) {
	//cout << "rebalance1: " << queue[index]->id << " ";
	//cout << "rebalance2: " << queue[index]->lifeTime << endl;
	//delete queue[1];
	//queue[1] = NULL;
	//if (size > 1) {
		for (int i = 1; i < index; i++) {		// if there are at least two items, primary array is shifted
			queue[i] = queue[i + 1];
		}
	//}
	if (index > 1) {
		//queue[index] = NULL;
	}
	index--;
}

void sendToBack(Process **queue, const int index) {
	Process *tmp = queue[1];
	int i;
	bool shifted = false;
	for (i = 1; i < index-1; i++) {
		queue[i] = queue[i + 1];
		shifted = true;
	}
	if (shifted) {
		queue[index-1] = tmp;
	}
	for (i = 1; i <= index; i++) {
	}

}

void roundRobin(Process **queue, Process **backLog, int &time, int size, int quantum, int &index, int &backupCtr, int &backupIndex, int totalTime) {
	bool flag = true, proc = true;
	int timeLimit = 0;
	while(flag){
		if (size == 1 && index > size) {
			index = 1;
		}
		if (time >= totalTime) {
			/*for (int j = 1; j <= backupCtr; j++) {
				delete backLog[j];
				backLog[j] = NULL;
			}*/
			//queue[1]->displayProperties(time, "end");
			flag = false;
			break;
		}
		timeLimit++;
		if (backupIndex <= backupCtr) {
			//cout << backLog[backupIndex]->arrivalTime << endl;
		}
		if (index < backupCtr && backupIndex <= backupCtr && backLog[backupIndex]->arrivalTime <= time) {
			queue[index++] = backLog[backupIndex++];
			if (index > size) {
				index = size;
			}
			if (backupIndex > backupCtr) {
				backupIndex = backupCtr;
			}
		}
		/*#arrival time, duration
			0, 8
			4, 2
			5, 3*/
		//cout << "a " << queue[1] << endl;
		if (!queue[1]->init) {
			queue[1]->displayProperties(time, "start");
			queue[1]->init = true;
		}
		if (queue[1]->lifeTime > 0) {
			for (int j = 0; j < quantum; j++) {
				//cout << queue[1]->lifeTime << endl;
				if (queue[1]->lifeTime < 1) {
					queue[1]->displayProperties(time, "end");
					rebalance(queue, backLog, index, size, backupCtr);
					if (index <= size && backupIndex <= backupCtr && backLog[backupIndex]->arrivalTime <= time) {
						queue[index++] = backLog[backupIndex++];
					}
					if (index > size) {
						index = size;
					}
					if (backupIndex > backupCtr) {
						backupIndex = backupCtr;
					}
					proc = false;
					break;
				}
				queue[1]->lifeTime--;
				time++;
				if (index <= size && backupIndex <= backupCtr && backLog[backupIndex]->arrivalTime <= time) {
					queue[index++] = backLog[backupIndex++];
				}
				if (queue[1]->lifeTime < 1) {
					queue[1]->displayProperties(time, "end");
					rebalance(queue, backLog, index, size, backupCtr);
					if (index <= size && backupIndex <= backupCtr && backLog[backupIndex]->arrivalTime <= time) {
						queue[index++] = backLog[backupIndex++];
					}
					if (index > size) {
						index = size;
					}
					if (backupIndex > backupCtr) {
						backupIndex = backupCtr;
					}
					proc = false;
					break;
				}
				queue[1]->displayProperties(time);
				proc = true;
			}
		}
		else {
				//queue[1]->displayProperties(time, "end");
				rebalance(queue, backLog, index, size, backupCtr);
				if (index < backupCtr && backupIndex <= backupCtr && backLog[backupIndex]->arrivalTime <= time) {
					queue[index++] = backLog[backupIndex++];
				}
				if (index > size) {
					index = size;
				}
				if (backupIndex > backupCtr) {
					backupIndex = backupCtr;
				}
				proc = false;
		}
		/*cout << endl << endl << "time: " << time << endl;
		for (int j = 1; j < index; j++) {
			cout << "address " << queue[j]->id << ": " << queue[j] << endl;
		}*/
		if (proc) {
			sendToBack(queue, index);
		}
	}
}

void buildArr(Process *arr[] , string str , int index) {	// builds array; parse str for arrival time and duration
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
	/*string params = "input=2.txt;size=2;scheduling=roundrobin;quantum=1";*/
	string params = argv[1];//
	string curParam;
	List<string> *paramList = new List<string>();
	int firstBreak = 0;
	while (firstBreak > -1) {						// keeps adding param segments to list
		firstBreak = params.find(';');
		curParam = params.substr(0, firstBreak);	// gets first segment; later added to end of list
		params = params.substr(firstBreak + 1);		// reassigns params to rest of the line

		paramList->insertAtEnd(curParam);
	}
	string fName, scheduling;		// declares variables 
	int size, quantum = 1;			// for params
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
	Process **queue = new Process*[size + 1];	// primary array
	Process **backLog;							// backlog array for excess processes
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
		txtLine.erase(remove(txtLine.begin(), txtLine.end(), '\r'), txtLine.end());
		words->insertSort(words->head , txtLine);	// builds list of lines
		actualSize++;								// counts number of lines
	}
	
	Node<string> *curLine = words->head;
	int j = 1;
	backLog = new Process*[actualSize + 1];
	while(curLine != NULL){						// starts building out both arrays
		string txtLine = curLine->data;
		buildArr(backLog, txtLine, j);		// backlog is built
		backLog[j]->id = j;
		totalTime += backLog[j]->duration;
		j++;
		curLine = curLine->next;
	}
	words->destroyDataList(words->head);				// free memory

	string header = "time processId duration action";
	cout << header << endl;
	int time = 0;
	j = 1;
	if (scheduling == "fifo") {
		while (j <= actualSize){
			int length = 0;
			for (int k = 0; k < size; k++) {		// refills queue with slots from backLog 
				queue[k%size + 1] = backLog[j];
				j++;
				length++;
				if (j > backupCtr) {
					break;
				}
			}
			fifo(queue, time, length);				// calls fifo() with next batch
		}
	}
	else {
		int index = 1;
		int backupIndex = 1;
		queue[index++] = backLog[backupIndex++];
		roundRobin(queue, backLog, time, size, quantum, index, actualSize, backupIndex, totalTime); // call roundRobin with necessary params
	}/**/
	return 0;
}