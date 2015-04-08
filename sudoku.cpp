#include <gecode/int.hh>
#include <gecode/minimodel.hh>
#include <gecode/search.hh>

#include "A1.cpp"

#define N 9

using namespace Gecode;

class Sudoku : public Space {
protected:
	IntVarArray cells;
public:
	Sudoku(int raw[N * N]) : cells(*this, N * N, 1, N) {
		// Initialize cells
		Matrix<IntVarArray> matrix(cells, N, N);

		// Post constraints
		// Fixed numbers
		for (int i = 0; i < N * N ; i++) {
			if (raw[i] != 0) {
				int row = i / N;
				int column = i % N;
				//std::cout << "Row: " << row << " Col: " << column << " Val: " << raw[i] << std::endl;
				rel(*this, matrix(column, row), IRT_EQ, raw[i]);
			}
		}

		// Distinct row
		for (int i = 0; i < N; i++)
			distinct(*this, matrix.row(i));
		// Distinct column
		for (int i = 0; i < N; i++)
			distinct(*this, matrix.col(i));
		// Distinct 3x3 block
		for (int i = 0; i < N; i += 3)
			for (int j = 0; j < N; j += 3)
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
	int raw[N * N];
	int counter = 0;

	for (int i = 0; i < N; i++) {
		for (int j = 0; j < N; j++) {
			int index = counter + j;
			raw[index] = examples[0][i][j];
		}
		counter += N;
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
