#include <gecode/int.hh>
#include <gecode/minimodel.hh>
#include <gecode/search.hh>
#include <gecode/driver.hh>

#include "A1.cpp"

#define N 9

using namespace Gecode;

class Sudoku : public Script {
protected:
	IntVarArray cells;
public:
	Sudoku(const Options& opt) : Script(opt), cells(*this, N * N, 1, N) {
		int raw[N * N];
		int counter = 0;

		// Flatten input matrix
		for (int i = 0; i < N; i++) {
			for (int j = 0; j < N; j++) {
				int index = counter + j;
				raw[index] = examples[0][i][j];
			}
			counter += N;
		}

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
		//std::cout << cells << std::endl;
	}
};

int main(int argc, char* argv[]) {
	/*int raw[N * N];
	int counter = 0;

	for (int i = 0; i < N; i++) {
		for (int j = 0; j < N; j++) {
			int index = counter + j;
			raw[index] = examples[0][i][j];
		}
		counter += N;
	}*/

	Options opt("Sudoku");
	opt.icl(ICL_DEF);
	opt.icl(ICL_DOM);
	opt.icl(ICL_VAL);
	opt.icl(ICL_BND);
	opt.parse(argc, argv);
	Script::run<Sudoku, BAB, Options>(opt);

	return 0;
}
