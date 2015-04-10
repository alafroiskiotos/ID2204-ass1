//
//  MatrixQueen.cpp
//  A1
//
//  Created by Lorenzo Corneo and Antonis Kouzoupis on 09/04/15.
//  Copyright (c) 2015 Lorenzo Corneo. All rights reserved.
//

#include <gecode/driver.hh>
#include <gecode/int.hh>
#include <gecode/minimodel.hh>

using namespace Gecode;

class MatrixQueen : public Script {
protected:
    // Array representing the linearization of the chessboard.
    IntVarArray chessBoardLin;
    const int n;
public:
    MatrixQueen(const SizeOptions& opt) : Script(opt), n(opt.size()), chessBoardLin(*this, opt.size() * opt.size(), 0, 1) {

        // Matrix representing the chessboard. Initialized to 0, but 1 means Queen in place.
        Matrix<IntVarArray> chessBoard(chessBoardLin, n, n);
        
        // Main diagonal contraint.
        linear(*this, getDiagonal(chessBoard, 0, 0, 0), IRT_LQ, 1);
        // Secondary diagonal constraint.
        linear(*this, getDiagonal(chessBoard, 0, n - 1, 1), IRT_LQ, 1);
        
        for(int i = 0; i < n; i++) {
            // Row contraint.
            linear(*this, chessBoard.row(i), IRT_EQ, 1);
            
            // Column contraint.
            linear(*this, chessBoard.col(i), IRT_EQ, 1);
            
            if(i > 0 && i < n - 1) {
                // Main diagonals constraint starting from the first row.
                linear(*this, getDiagonal(chessBoard, 0, i, 0), IRT_LQ, 1);
                // Main diagonals constraint starting from the first column.
                linear(*this, getDiagonal(chessBoard, i, 0, 0), IRT_LQ, 1);
                // Secondary diagonal constraint starting from the last column.
                linear(*this, getDiagonal(chessBoard, i, n - 1, 1), IRT_LQ, 1);
                // Secondary diagonal constraint starting from the first row.
                linear(*this, getDiagonal(chessBoard, 0, i, 1), IRT_LQ, 1);
                // Secondary diagonal constraint starting from the first column.
                linear(*this, getDiagonal(chessBoard, i, 0, 1), IRT_LQ, 1);
            }
        }
        
        branch(*this, chessBoardLin, INT_VAR_SIZE_MIN(), INT_VAL_MAX());
    }

    // Returns the diagonal (main or secondary) belonging to a cell given as a input.
    // selector = 0 -> main diagonal, else -> secondary diagonal
    IntVarArgs getDiagonal(Matrix<IntVarArray> m, int row, int col, int selector) {
        IntVarArgs diagonal;
        
        // Sets the starting indexes.
        int rowIndex = diagonalIndex(0, selector, row, col);
        int colIndex = diagonalIndex(1, selector, row, col);
        
        while(rowIndex < n && colIndex >= 0 && colIndex < n) {
            diagonal << m(rowIndex, colIndex);
            rowIndex++;
            if(selector == 0) {
                colIndex++;
            } else {
                colIndex--;
            }
        }
        
        // std::cout << "{" << row << ", " << col << "} " << diagonal << std::endl;
        
        return diagonal;
    }
    
    // Return the starting index of column and row.
    // In a nut, the starting indexes of the diagonal belonginng to a generic cell
    // indexSelector 0 -> return row, indexSelector != 0 -> return column
    // diagonalSelector = 0 -> main diagonal, else -> secondary diagonal
    int diagonalIndex(int indexSelector, int diagonalSelector, int row, int col) {
        if(row == 0 || col == 0 || row == n - 1 || col == n - 1) {
            return indexSelector == 0 ? row : col;
        }
        
        do {
            row--;
            if(diagonalSelector == 0) {
                col--;
            } else {
                col++;
            }
        } while(row > 0 && (col > 0 && col < n));
        
        return indexSelector == 0 ? row : col;
    }
    
    /// Constructor for cloning \a s
    MatrixQueen(bool share, MatrixQueen& s, int size) : Script(share,s), n(size) {
        chessBoardLin.update(*this, share, s.chessBoardLin);
    }
    
    /// Perform copying during cloning
    virtual Space*
    copy(bool share) {
        return new MatrixQueen(share,*this,n);
    }
    
    virtual void
    print(std::ostream& os) const {
        for(int i = 1; i <= n * n; i++) {
            std::cout << chessBoardLin[i-1] << " ";
            
            if(i % n == 0) {
                std::cout << std::endl;
            }
        }
    }
};

int main(int argc, char* argv[]) {
        SizeOptions opt("MatrixQueens");
        opt.size(4);
        opt.parse(argc, argv);
        Script::run<MatrixQueen,BAB,SizeOptions>(opt);
    return 0;
}
