#pragma once
#ifndef MULTIPLYMATRIX_H
#define MULTIPLYMATRIX_H
#include "Record.h"
#include "List.h"

using namespace std;

// MULTIPLICATION ALGORITHM TAKEN/INTERPRETED FROM THE FOLLOWING POST BY THE FOLLOWING POST BY Aaron Johnson ON THE GOOGLE GROUP
// https://groups.google.com/d/msg/cosc2320-f2015/dzgSPibAieQ/YGHm7I0DCgAJ
int recArrMult(Record<int> recsRow[], Record<int> recsCol[], int arrCtr1, int arrCtr2, List<Record<int> > *recList) {
	int prodEdgeCtr = 0;
	for (int i = 0; i < arrCtr1; i++) {
		int sum = 0;
		for (int j = 0; j < arrCtr2; j++) {
			if (recsRow[i].col == recsCol[j].row && recsRow[i].row != recsCol[j].col) {// CHECKS THAT THE COL VALUE IN THE ROW-SORTED ARRAY MATCHES THE ROW VALUE OF THE COL-SORTED ARRAY AND THAT THE ROW AND COL OF THE RESPECTIVE ARRAYS DO NOT MATCH (IDENTITY DIAGONAL)
				Record<int> temp(recsRow[i].row, recsCol[j].col);
				recList->insertAtEnd(temp);
				prodEdgeCtr++;
			}
		}
	}
	return prodEdgeCtr;
}

#endif