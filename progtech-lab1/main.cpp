#include <iostream>
#include <vector>
#include <iomanip>
#include <limits>
#include <fstream>

#define DEBUG

class CCSMatrix {
	std::vector<int> values;
	std::vector<int> rowIndexes;
	std::vector<int> colPointers;
	int rows, cols;

private:
	int calcRowSum(int row) {
		// Функция, которая возвращает сумму значений строки. 
		// Аргумент:
		// row - номер строки.
		int sum = 0;
		for (int i = 0; i < rowIndexes.size(); i++) {
			if (rowIndexes[i] == row) {
				sum += values[i];
			}
		}
		return sum;
	}

	void swapRows(int row1, int row2) {
		// Функция, меняющая местами две строки.
		// Аргументы:
		// row1: индекс первой строки
		// row2: индекс второй строки
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
	void packMatrix(const std::vector<std::vector<int>>& matrix) {
		// Упаковка матрицы в формат CCS
		// Аргументы:
		// const std::vector<std::vector<int>>&: неупакованная матрица
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

public:
	CCSMatrix(std::istream& input) {
		// Конструктор матрицы из потока
		// Аргументы:
		// std::istream& input: поток, откуда будет читаться матрица

		input >> rows;
		input >> cols;

		colPointers.resize(cols + 1, 0);

		std::vector<std::vector<int>> tempValues(cols);
		std::vector<std::vector<int>> tempRows(cols);


		for (int row = 0; row < rows; row++) {
			for (int col = 0; col < cols; col++) {
				int value;

				input >> value;

				if (value != 0) {
					tempValues[col].push_back(value);
					tempRows[col].push_back(row);
				}
			}
		}

		for (int col = 0; col < cols; col++) {
			for (int i = 0; i < tempValues[col].size(); i++) {
				values.push_back(tempValues[col][i]);
				rowIndexes.push_back(tempRows[col][i]);
			}

			colPointers[col + 1] = values.size();
		}
	}

	void printPacked() {
		// Функция, выводящая в консоль матрицу в упакованном формате
		std::cout << std::left;
		std::cout << std::setw(20) << "Values: ";
		for (int x : values) {
			std::cout << std::setw(4) << x;
		}
		std::cout << std::endl;
		std::cout << std::setw(20) << "RowIndexes: ";
		for (int x : rowIndexes) {
			std::cout << std::setw(4) << x;
		}
		std::cout << std::endl;
		std::cout << std::setw(20) << "ColumnPointers: ";
		for (int x : colPointers) {
			std::cout << std::setw(4) << x;
		}
		std::cout << std::endl;
		std::cout << std::right;
	}

	void print() {
		// Функция, выводящая в консоль матрицу в привычном формате
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
		// Функция сортировки матрицы.
		for (int i = 0; i < rows; i++) {
			int smallestRow = i;

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
	while (true) {
		std::cout << "Where do I get the matrix? \n1 - console\n2 - file\n0 - exit\n\nEnter: ";
		int switchValue;
		std::cin >> switchValue;
		CCSMatrix* ccsMatrix = nullptr;
		switch (switchValue) {
			case 1: 
			{
				std::cout << "Enter the matrix: ";
				ccsMatrix = new CCSMatrix(std::cin);
				break;
			}
			case 2: {
				std::string filename;
				std::cout << "Enter the filename: ";
				std::cin >> filename;
				std::ifstream file(filename);
				ccsMatrix = new CCSMatrix(file);
				file.close();
				break;
			}
			default: {
				return 0;
			}
		}


		std::cout << "\nUnpacked matrix: \n";
		ccsMatrix->print();
		std::cout << std::endl;
		std::cout << "Packed matrix: \n";
		ccsMatrix->printPacked();
		std::cout << std::endl;
		ccsMatrix->sort();
		std::cout << "\nUnpacked sorted matrix: \n";
		ccsMatrix->print();
		std::cout << std::endl;

		
		std::cout << "Press Enter to continue...\n";
		std::cin.get();
		std::cin.get();

		delete ccsMatrix;
		system("cls");
	}
}