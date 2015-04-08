/*
 * sudoku.cpp
 *
 *  Created on: Apr 8, 2015
 *      Author: antonis
 */

#include <gecode/int.hh>
#include <gecode/search.hh>

using namespace Gecode;

class Sudoku : public Space {
protected:
	IntVarArray cells;
public:
	Sudoku(void) : cells(*this) {
		// Initialize cells

		// Post constraints

		// Branching
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
	// Create model
	Sudoku* sudoku = new Sudoku;

	// Create search engine
	BAB<Sudoku> e(sudoku);
	delete(sudoku);

	while (Sudoku* solution = e.next()) {
		solution -> print();
		delete(solution);
	}

	return 0;
}
