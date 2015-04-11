#include <gecode/int.hh>
#include <gecode/minimodel.hh>
#include <gecode/search.hh>
#include <gecode/driver.hh>

#include "A1.cpp"

#define N 9

using namespace Gecode;

// Custom options handler
class PuzzleOptions : public Options {
private:
	Driver::UnsignedIntOption _puzzle;
public:
	PuzzleOptions(const char* e) : Options(e), _puzzle("-puzzle", "Example puzzle number [0-17]", 0) {
		add(_puzzle);
	}
	unsigned int puzzle(void) const {
		return _puzzle.value();
	}

	void parse(int& argc, char* argv[]) {
		Options::parse(argc, argv);
	}
};

class Sudoku : public Script {
protected:
	IntVarArray cells;
private:
	int raw[N * N];
	int counter;
	int puzzle;
public:
	Sudoku(const PuzzleOptions& opt) : Script(opt), cells(*this, N * N, 1, N), puzzle(opt.puzzle()) {
		std::cout << "Example: " << puzzle << std::endl;
		counter = 0;
		// Flatten input matrix
		for (int i = 0; i < N; i++) {
			for (int j = 0; j < N; j++) {
				int index = counter + j;
				raw[index] = examples[puzzle][i][j];
			}
			counter += N;
		}

		// Initialize cells
		Matrix<IntVarArray> matrix(cells, N, N);

		// Post constraints
		// Fixed numbers
		for (int i = 0; i < N * N ; i++) {
			if (raw[i] != 0) {
				// Find corresponding row and column
				int row = i / N;
				int column = i % N;
				rel(*this, matrix(column, row), IRT_EQ, raw[i], opt.icl());
			}
		}

		// Distinct row
		for (int i = 0; i < N; i++)
			distinct(*this, matrix.row(i), opt.icl());
		// Distinct column
		for (int i = 0; i < N; i++)
			distinct(*this, matrix.col(i), opt.icl());
		// Distinct 3x3 block
		for (int i = 0; i < N; i += 3)
			for (int j = 0; j < N; j += 3)
				distinct(*this, matrix.slice(i, i + 3, j, j + 3), opt.icl());

		// Branching
		branch(*this, cells, INT_VAR_MIN_MAX(), INT_VAL_MED());
	}

	// Constructor for cloning
	Sudoku(bool share, Sudoku& s) : Script(share, s) {
		cells.update(*this, share, s.cells);
	}

	// Perform copying
	virtual Space* copy(bool share) {
		return new Sudoku(share, *this);
	}

	// Print solution, Command Prompt
	virtual void print(std::ostream& os) const {
		os << "=================================" << std::endl;
		for (int i = 1; i <= N*N; i++) {
			os << " " << cells[i-1] << " ";
			if ((i % 3) == 0 && i != 0)
				os << "||";
			if (((i % N) == 0) && (i != 0)) {
				os << std::endl;
				if ((i % 27) == 0 && i != 0) {
					os << "=================================" << std::endl;
				}
			}
		}
		os << std::endl;
	}
};

int main(int argc, char* argv[]) {
	PuzzleOptions opt("Sudoku");

	opt.parse(argc, argv);
	Script::run<Sudoku, BAB, PuzzleOptions>(opt);

	return 0;
}
