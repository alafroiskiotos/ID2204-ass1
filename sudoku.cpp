/*
 * sudoku.cpp
 *
 *  Created on: Apr 8, 2015
 *      Author: antonis
 */

#include <gecode/int.hh>
#include <gecode/minimodel.hh>
#include <gecode/search.hh>

#include "A1.cpp"

using namespace Gecode;

class Sudoku : public Space {
protected:
	IntVarArray cells;
public:
	Sudoku(int raw[9 * 9]) : cells(*this, 9*9, 1, 9) {

		/*for (int i = 0; i < 9 * 9; i++) {
			std::cout << raw[i];
			if ((i % 9) == 0)
				std::cout << std::endl;
		}*/
		// Initialize cells
		Matrix<IntVarArray> matrix(cells, 9, 9);

		// Post constraints

		// Fixed numbers
		for (int i = 0; i < 9*9 ; i++) {
			if (raw[i] != 0) {
				int row = i / 9;
				int column = i % 9;
				std::cout << "Row: " << row << " Col: " << column << " Val: " << raw[i] << std::endl;
				rel(*this, matrix(column, row), IRT_EQ, raw[i]);
			}
		}
		Slice<IntVarArray> lala = matrix.row(2);
		std::cout << lala << std::endl;

		// Distinct row
		for (int i = 0; i < 9; i++)
			distinct(*this, matrix.row(i));
		// Distinct column
		for (int i = 0; i < 9; i++)
			distinct(*this, matrix.col(i));
		// Distinct 3x3 block
		for (int i = 0; i < 9; i += 3)
			for (int j = 0; j < 9; j += 3)
				distinct(*this, matrix.slice(i, i + 3, j, j + 3));


		// Branching
		branch(*this, cells, INT_VAR_SIZE_MIN(), INT_VAL_MIN());
	}

	// Constructor for cloning
	Sudoku(bool share, Sudoku& s) : Space(share, s) {
		cells.update(*this, share, s.cells);
	}

	// Perform copying
	virtual Space* copy(bool share) {
		return new Sudoku(share, *this);
	}

	// Print solution, Command Prompt
	void print(void) {
		std::cout << cells << std::endl;
	}
};

int main(int argc, char* argv[]) {
	int raw[9 * 9];
	int counter = 0;

	for (int i = 0; i < 9; i++) {
		for (int j = 0; j < 9; j++) {
			int index = counter + j;
			raw[index] = examples[0][i][j];
		}
		counter += 9;
	}
	// Create model
	Sudoku* sudoku = new Sudoku(raw);

	// Create search engine
	DFS<Sudoku> e(sudoku);
	delete(sudoku);

	while (Sudoku* solution = e.next()) {
		solution -> print();
		delete(solution);
	}

	return 0;
}
