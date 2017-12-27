#include <iostream>
#include <fstream>
#include <sstream>
#include <locale>
#include <algorithm>
#include <string>
#include <cstdlib>
#include "multiplymatrix.h"
#include "sortmatrix.h"

using namespace std;

void reconcileList(List<Record<int> > *list, int &ctr) { // takes an unsorted list and combines duplicates/deletes excess Records
	Node<Record<int> > *cur = list->head;
	Node<Record<int> > *head = list->head;

	while (cur != NULL || cur != 0) {
		Node<Record<int> > *temp = head;

		while (temp != NULL) {
			Node<Record<int> > *inner = temp->next;
			if (cur->data.row == temp->data.row && cur->data.col == temp->data.col && cur != temp) {
				cur->data.val += temp->data.val;
				list->deleteThisNode(temp);
				ctr--;
			}
			temp = inner;
		}
		cur = cur->next;
	}
}

int recArrAdd(Record<int> arr[],int ctr, List<Record<int> > *list){ // Just dumps E**x values to a master list that is then called with reconcileList();
	bool flag = true;
	int i = 0;
	for (i; i < ctr; i++){
		list->insertAtEnd(arr[i]);
	}
	return i;
}

int getMax(int arr[] , int ctr) {
	int ind = 0;
	for (int i = 0; i < ctr; i++){
		if (arr[i] > arr[ind]){
			ind = i;
		}
	}
	return ind;
}

int getIndex(string arr[], string page , int ctr , int start , int end){ // does a binary search that returns the index of the page that is used for building the row and col values of the "matrices"
	if (start > end) {
		return -1;
	}
	else {
		int mid = (start + end) / 2;
		if (arr[mid] < page) {
			return getIndex(arr, page, ctr, mid+1, end);
		}
		else if (arr[mid] > page) {
			return getIndex(arr, page, ctr, start, mid-1);
		}
		else {
			return mid;
		}
	}
}

int parseInt(string str) {			// returns int value of provided string
	int holder;
	stringstream ss(str);
	ss >> holder;
	return holder;
}

int main(int argc, char *argv[]) {
	string paramStr = argv[1];
	int pos = paramStr.find(';');
	string fileName = paramStr.substr(0, pos);
	string sort = paramStr.substr(pos + 1);
	pos = fileName.find('=');
	fileName = fileName.substr(pos + 1);
	pos = sort.find('=');
	sort = sort.substr(pos + 1);

	fstream listIn(fileName.c_str());
	if (!listIn.good()){
		cout << fileName << " could not be found." << endl;
		exit(0);
	}
	if (listIn.peek() == EOF) {
		cout << "input file " << fileName << " is empty" << endl;
		exit(0);
	}
	string logFile = fileName + "-log.txt";
	ofstream fout(logFile.c_str(), ios::out | ios::app);
	string pageName, pageLine, link;
	int linkStartPos, linkEndPos;
	List<string> *pageList = new List<string>();
	int ctr = 0;
	while (getline(listIn, pageName)) {
		pageName.erase(remove(pageName.begin(), pageName.end(), '\r'), pageName.end()); // got this line from a StackOverflow question on how to remove the EOL character from a string. It's used every now and then further on
		pageList->insertAtEnd(pageName);
		ctr++;
	}
	int **vertexArr;
	vertexArr = new int*[ctr];
	for (int i = 0; i < ctr; i++) {
		vertexArr[i] = new int[ctr];
		for (int j = 0; j < ctr; j++) {
			vertexArr[i][j] = 0;
		}
	}
	Node<string> *cur = pageList->head;
	string *pageNamesArr = new string[ctr];
	string *workArr = new string[ctr];
	int pageInd = 0;
	while (cur != NULL){
		pageNamesArr[pageInd++] = cur->data;
		cur = cur->next;
	}
	cur = NULL;
	pageList->destroyList(pageList->head);
	if (sort == "slow") {
		bubbleSort(pageNamesArr, pageInd);
	} 
	else {
		mergeSort(pageNamesArr, workArr, 0, ctr);
	}
	List<Record<int> > *E1recList = new List<Record<int> >();
	List<Record<int> > *E2recList = new List<Record<int> >();
	List<Record<int> > *E3recList = new List<Record<int> >();
	int E1edgeCtr = 0, E2edgeCtr = 0 , E3edgeCtr = 0;
	for (int i = 0; i < ctr; i++){
		fstream pageIn(pageNamesArr[i].c_str());
		if (pageIn.peek() == EOF) {
			fout << pageNamesArr[i] << " file is empty." << endl;
			continue;
		}
		while (getline(pageIn, pageLine)) {
			pageLine.erase(remove(pageLine.begin(), pageLine.end(), '\r'), pageLine.end());
			if (pageLine.find("<a href") == std::string::npos){
				continue;
			}
			linkStartPos = pageLine.find('"');
			link = pageLine.substr(linkStartPos + 1);
			linkEndPos = link.find('"');
			if (linkEndPos < 3) {
				continue;
			}
			link = link.substr(0, linkEndPos);
			int pageNum = getIndex(pageNamesArr, link, ctr, 0, ctr-1);
			if (pageNum == -1) {
				fout << link << " is not in case group." << endl;
				continue;
			}
			Record<int> temp = Record<int>(i, pageNum);
			
			E1recList->insertAtEnd(temp);
			vertexArr[i][pageNum]++;
			E1edgeCtr++;
		}
	}
	Record<int> *E1recsRow = new Record<int>[E1edgeCtr];
	Record<int> *E1recsCol = new Record<int>[E1edgeCtr];
	Node<Record<int> > *curRec = E1recList->head;
	int recInd = 0;
	// push Records to array for sorting
	while (curRec != NULL) {
		E1recsRow[recInd] = curRec->data;
		E1recsCol[recInd] = curRec->data;
		curRec = curRec->next;
		recInd++;
	}
	//cout << "b" << endl;
	E1recList->destroyList(E1recList->head);
	curRec = NULL;
	if (sort == "slow"){
		bubbleSortRow(E1recsRow, E1edgeCtr);
		bubbleSortCol(E1recsCol, E1edgeCtr);
	}
	else {
		Record<int> *temp = new Record<int>[E1edgeCtr];
		mergeSortRow(E1recsRow, temp, 0, E1edgeCtr);
		mergeSortCol(E1recsCol, temp, 0, E1edgeCtr);
		delete[] temp;
	}

	// START OF E2 CALCULATIONS
	E2edgeCtr = recArrMult(E1recsRow, E1recsCol, E1edgeCtr, E1edgeCtr, E2recList);
	curRec = E2recList->head;
	Record<int> *E2recsRow = new Record<int>[E2edgeCtr];
	Record<int> *E2recsCol = new Record<int>[E2edgeCtr];
	curRec = E2recList->head;
	// push Records to array for sorting
	for (int i = 0; i < E2edgeCtr; i++){
		E2recsRow[i] = curRec->data;
		E2recsCol[i] = curRec->data;
		curRec = curRec->next;
	}
	if (sort == "slow"){
		bubbleSortRow(E2recsRow, E2edgeCtr);
		bubbleSortCol(E2recsCol, E2edgeCtr);
	}
	else {
		Record<int> *temp = new Record<int>[E2edgeCtr];
		mergeSortRow(E2recsRow, temp, 0, E2edgeCtr);
		mergeSortCol(E2recsCol, temp, 0, E2edgeCtr);
		delete[] temp;
	} 

	E3edgeCtr = recArrMult(E2recsRow, E1recsCol, E2edgeCtr, E1edgeCtr, E3recList);
	reconcileList(E3recList, E3edgeCtr);
	curRec = E3recList->head;

	curRec = E3recList->head;
	Record<int> *E3recsRow = new Record<int>[E3edgeCtr];
	Record<int> *E3recsCol = new Record<int>[E3edgeCtr];
	// push Records to array for sorting
	for (int i = 0; i < E3edgeCtr; i++){
		E3recsRow[i] = curRec->data;
		E3recsCol[i] = curRec->data;
		curRec = curRec->next;
	}
	E3recList->destroyList(E3recList->head);
	//bubbleSortCol(E3recsCol, E3edgeCtr);
	
	if (sort == "slow"){
		bubbleSortRow(E3recsRow, E3edgeCtr);
		bubbleSortCol(E3recsCol, E3edgeCtr);
	}
	else {
		Record<int> *temp = new Record<int>[E3edgeCtr];
		mergeSortRow(E3recsRow, temp, 0, E3edgeCtr);
		mergeSortCol(E3recsCol, temp, 0, E3edgeCtr);
		delete[] temp;
	}
	List<Record<int> > *GList = new List<Record<int> >();
	int GedgeCtr = recArrAdd(E1recsRow, E1edgeCtr, GList);
	GedgeCtr += recArrAdd(E2recsRow, E2edgeCtr, GList);
	GedgeCtr += recArrAdd(E3recsRow, E3edgeCtr, GList);
	reconcileList(GList, GedgeCtr);
	Record<int> *GarrRow = new Record<int>[GedgeCtr];
	Record<int> *GarrCol = new Record<int>[GedgeCtr];
	curRec = GList->head;
	int i = 0;
	// push Records to array for sorting
	while (curRec != NULL){
		GarrRow[i] = curRec->data;
		GarrCol[i] = curRec->data;
		i++;
		curRec = curRec->next;
	}

	Record<int> *GrecArr = new Record<int>[GedgeCtr];
	curRec = GList->head;
	int gCtr = 0;
	while (curRec != NULL) {
		GrecArr[gCtr] = curRec->data;
		gCtr++;
		curRec = curRec->next;
	}
	if (sort == "slow"){
		bubbleSortRow(GarrRow, GedgeCtr);
		bubbleSortCol(GarrCol, GedgeCtr);
	}
	else {
		Record<int> *temp = new Record<int>[GedgeCtr];
		mergeSortRow(GarrRow, temp, 0, GedgeCtr);
		mergeSortCol(GarrCol, temp, 0, GedgeCtr);
	}

	/*for (int j = 0; j < GedgeCtr; j++) {
		cout << GarrRow[j].row << " " << GarrRow[j].col << " " << GarrRow[j].val << endl;
	}

	cout << endl;

	for (int j = 0; j < GedgeCtr; j++) {
		cout << GarrCol[j].row << " " << GarrCol[j].col << " " << GarrCol[j].val << endl;
	}*/

	gCtr = 0;
	int *outDegArr = new int[pageInd];
	// outDegArr for getting sums of each row
	for (int i = 0; i < pageInd; i++){
		outDegArr[i] = 0;
		while (i == GarrRow[gCtr].row){
			outDegArr[i] += GarrRow[gCtr++].val;
		}
	}
	gCtr = 0;
	int *inDegArr = new int[pageInd];
	// outDegArr for getting sums of each col
	for (int i = 0; i < pageInd; i++){
		inDegArr[i] = 0;
		while (i == GarrCol[gCtr].col){
			inDegArr[i] += GarrCol[gCtr++].val;
		}
	}
	
	// call getMax() to find array index with the highest count
	int outDegree = getMax(outDegArr, pageInd);
	// index used for pageNamesArr that was build in the beginning
	cout << "top outgoing connections web page=" << pageNamesArr[outDegree] << endl;
	// call getMax() to find array index with the highest count
	int inDegree = getMax(inDegArr, pageInd);
	// index used for pageNamesArr that was build in the beginning
	cout << "top incoming connections web page=" << pageNamesArr[inDegree] << endl;
	
	return 0;
}