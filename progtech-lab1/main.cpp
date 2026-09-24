#include <iostream>
#include <vector>
#include <iomanip>
#include <limits>

#define DEBUG

class CCSMatrix {
	std::vector<int> values;
	std::vector<int> rowIndexes;
	std::vector<int> colPointers;
	int rows, cols;

private:
	int calcRowSum(int row) {
		int sum = 0;
		for (int i = 0; i < rowIndexes.size(); i++) {
			if (rowIndexes[i] == row) {
				sum += values[i];
			}
		}
		return sum;
	}

	void swapRows(int row1, int row2) {
		std::vector<int> tempRow1;
		for (int i = 0; i < rowIndexes.size(); i++) {
			if (rowIndexes[i] == row1) {
				rowIndexes[i] = -1;
			}
		}
		for (int i = 0; i < rowIndexes.size(); i++) {
			if (rowIndexes[i] == row2) {
				rowIndexes[i] = row1;
			}
		}

		for (int i = 0; i < rowIndexes.size(); i++) {
			if (rowIndexes[i] == -1) {
				rowIndexes[i] = row2;
			}
		}

#ifdef DEBUG
		std::cout << "Swapped: " << row1 << ", " << row2 << std::endl;
#endif
	}

public:

	CCSMatrix(int** matrix, int rows, int cols): rows(rows), cols(cols) {
	// Упаковка матрицы в формат CCS
		colPointers.resize(cols + 1, 0);
		for (int curCol = 0; curCol < cols; curCol++) {
			for (int curRow = 0; curRow < rows; curRow++) {
				if (matrix[curRow][curCol] != 0) {
					values.push_back(matrix[curRow][curCol]);
					rowIndexes.push_back(curRow);
				}
			}
			colPointers[curCol + 1] = values.size();
		}
	}

	~CCSMatrix() {
	}

	void print() {
		std::cout << std::left;
		for (int rowId = 0; rowId < rows; rowId++) {
			for (int colId = 0; colId < cols; colId++) {
				int colDataStartId = colPointers[colId];
				int colDataEndId = colPointers[colId + 1];

				int value = 0;

				for (int i = colDataStartId; i < colDataEndId; i++) {
					if (rowIndexes[i] == rowId) {
						value = values[i];
						break;
					}
				}

				std::cout << std::setw(4) << value;
			}
#ifdef DEBUG
			std::cout << "Sum: " << calcRowSum(rowId);
#endif
			std::cout << std::endl;
		}
		std::cout << std::right;
	}

	void sort() {
		for (int i = 0; i < rows; i++) {
			int smallestRow = -1;

			int minSum = calcRowSum(i);
			int curSum;
			for (int j = i+1; j < rows; j++) {
				curSum = calcRowSum(j);
				if (curSum < minSum) {
					minSum = curSum;
					smallestRow = j;
				}
			}

			if (smallestRow != i) {
				swapRows(i, smallestRow);
			}
		}
	}

};

int main() {
	int rows = 3;
	int cols = 3;

	int** matrix = new int* [rows];

	for (int i = 0; i < rows; i++) {
		matrix[i] = new int[cols];
	}

	matrix[0][0] = 0;
	matrix[0][1] = 0;
	matrix[0][2] = 1;

	matrix[1][0] = 2;
	matrix[1][1] = 3;
	matrix[1][2] = 543;

	matrix[2][0] = 0;
	matrix[2][1] = 148;
	matrix[2][2] = 3;

	CCSMatrix* ccsMatrix = new CCSMatrix(matrix, 3, 3);
	ccsMatrix->print();
	ccsMatrix->sort();
	ccsMatrix->print();
}