#pragma once
#ifndef SORTMATRIX_H
#define SORTMATRIX_H
#include <string>
#include <iostream>
#include "List.h"
#include "Record.h"
#include "Node.h"

using namespace std;

// START MERGE SORT FOR "FAST" PARAM

/*
	Merge sort algorithm taken from here and modified to sort by both row && col within the same array.
*/

// START OF MERGE SORT FOR PAGENAMESARR
void merge(string arr1[], string arr2[], int start, int mid, int end) {
	int ctr1 = start;
	int ctr2 = mid;
	for (int i = start; i < end; i++) {
		if (ctr1 < mid && (ctr2 >= end || arr1[ctr1] <= arr1[ctr2])) {
			arr2[i] = arr1[ctr1];
			ctr1++;
		}
		else {
			arr2[i] = arr1[ctr2];
			ctr2++;
		}
	}
}

void copyArr(string arr1[], string arr2[], int start, int end) {
	for (int i = start; i < end; i++) {
		arr1[i] = arr2[i];
	}
}

void mergeSort(string arr1[], string arr2[], int start, int end) {
	if (end - start <= 1) {
		return;
	}
	int mid = (end + start) / 2;
	mergeSort(arr1, arr2, start, mid);
	mergeSort(arr1, arr2, mid, end);
	merge(arr1, arr2, start, mid, end);
	copyArr(arr1, arr2, start, end);
}

// END OF MERGE SORT FOR PAGENAMESARR

// START OF MERGE SORT FOR MATRICES

void mergeRow(Record<int> *arr1, Record<int> *arr2, int start, int mid, int end) {
	int ctr1 = start;
	int ctr2 = mid;
	for (int i = start; i < end; i++) {
		if (ctr1 < mid && (ctr2 >= end || arr1[ctr1].row < arr1[ctr2].row || (arr1[ctr1].row == arr1[ctr2].row && arr1[ctr1].col < arr1[ctr2].col))) {
			arr2[i] = arr1[ctr1];
			ctr1++;
		}
		else {
			arr2[i] = arr1[ctr2];
			ctr2++;
		}
	}
}

void mergeCol(Record<int> *arr1, Record<int> *arr2, int start, int mid, int end) {
	int ctr1 = start;
	int ctr2 = mid;
	for (int i = start; i < end; i++) {
		if (ctr1 < mid && (ctr2 >= end || arr1[ctr1].col < arr1[ctr2].col || (arr1[ctr1].col == arr1[ctr2].col && arr1[ctr1].row < arr1[ctr2].row))) {
			arr2[i] = arr1[ctr1];
			ctr1++;
		}
		else {
			arr2[i] = arr1[ctr2];
			ctr2++;
		}
	}
}

void copyArr(Record<int> *arr1, Record<int> *arr2, int start, int end) {
	for (int i = start; i < end; i++) {
		arr1[i] = arr2[i];
	}
}

void mergeSortRow(Record<int> *arr1, Record<int> *arr2, int start, int end) {
	if (end - start <= 1) {
		return;
	}
	int mid = (end + start) / 2;
	mergeSortRow(arr1, arr2, start, mid);
	mergeSortRow(arr1, arr2, mid, end);
	mergeRow(arr1, arr2, start, mid, end);
	copyArr(arr1, arr2, start, end);
}

void mergeSortCol(Record<int> *arr1, Record<int> *arr2, int start, int end) {
	if (end - start <= 1) {
		return;
	}
	int mid = (end + start) / 2;
	mergeSortCol(arr1, arr2, start, mid);
	mergeSortCol(arr1, arr2, mid, end);
	mergeCol(arr1, arr2, start, mid, end);
	copyArr(arr1, arr2, start, end);
}

// END OF MERGE SORT FOR MATRICES

// START BUBBLE SORT FOR "SLOW" PARAM
/*
	Bubble functions take/modified from this address:
	http://www.cplusplus.com/forum/general/127295/
*/
void bubbleSort(string arr[], int ctr) { // SORTS PAGENAMESARR
	for (int i = 0; i < ctr - 1; i++) {
		for (int j = 0; j < ctr - i - 1; j++) {
			if (arr[j] > arr[j + 1]) {
				string temp = arr[j];
				arr[j] = arr[j + 1];
				arr[j + 1] = temp;
			}
		}
	}
}

void bubbleSortRow(Record<int> arr[], int ctr) { // SORT BY ROW THEN BY COL
	for (int i = 0; i < ctr - 1; i++) {
		for (int j = 0; j < ctr - i - 1; j++) {
			if (arr[j].row > arr[j + 1].row || (arr[j].row == arr[j + 1].row && arr[j].col > arr[j + 1].col)) {
				Record<int> temp = arr[j];
				arr[j] = arr[j + 1];
				arr[j + 1] = temp;
			}
		}
	}
}

void bubbleSortCol(Record<int> arr[], int ctr) { // SORT COL THEN BY ROW
	for (int i = 0; i < ctr-1; i++) {
		for (int j = 0; j < ctr - i - 1; j++) {
			if (arr[j].col > arr[j + 1].col || (arr[j].col == arr[j + 1].col && arr[j].row > arr[j + 1].row)) {
				Record<int> temp = arr[j];
				arr[j] = arr[j + 1];
				arr[j + 1] = temp;
			}
		}
	}
}

#endif