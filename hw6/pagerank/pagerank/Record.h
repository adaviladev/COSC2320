#pragma once


template<typename S>
struct Record {
	int row;
	int col;
	int val;

	Record() {
		row = 0;
		col = 0;
		val = 0;
	}
	Record(S newRow, S newCol, S newVal = 1) {
		row = newRow;
		col = newCol;
		val = newVal;
	}
};