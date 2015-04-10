CXX=g++
CXXFLAGS=-O1 -Wall
LIBS=-lgecodeflatzinc -lgecodedriver -lgecodegist -lgecodesearch -lgecodeminimodel \
	-lgecodeset -lgecodefloat -lgecodeint -lgecodekernel -lgecodesupport
RM=rm
SRCS0=sudoku.cpp A1.cpp
SRCS1=MatrixQueen.cpp
OBJS0=$(subst .cpp,.o,$(SRCS0))
OBJS1=$(subst .cpp,.o,$(SRCS1))
EXEC=sudoku matrix_queen

all: $(EXEC)

sudoku: $(OBJS0)
	$(CXX) $(CXXFLAGS) -o sudoku $(OBJS0) $(LIBS)

sudoku.o: sudoku.cpp

matrix_queen: $(OBJS1)
	$(CXX) $(CXXFLAGS) -o matrix_queen $(OBJS1) $(LIBS)

MatrixQueen.o: MatrixQueen.cpp

clean:
	$(RM) $(OBJS0) $(OBJS1)

dist-clean: clean
	$(RM) $(EXEC)
