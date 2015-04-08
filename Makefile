CXX=g++
CXXFLAGS=-O1 -Wall
LIBS=-lgecodeflatzinc -lgecodedriver -lgecodegist -lgecodesearch -lgecodeminimodel \
	-lgecodeset -lgecodefloat -lgecodeint -lgecodekernel -lgecodesupport
RM=rm
SRCS0=sudoku.cpp A1.cpp
#SRCS1=donald.cpp
OBJS0=$(subst .cpp,.o,$(SRCS0))
#OBJS1=$(subst .cpp,.o,$(SRCS1))
EXEC=sudoku

all: $(EXEC)

sudoku: $(OBJS0)
	$(CXX) $(CXXFLAGS) -o sudoku $(OBJS0) $(LIBS)

money.o: money.cpp

#donald: $(OBJS1)
#	$(CXX) $(CXXFLAGS) -o donald $(OBJS1) $(LIBS)

#donald.o: donald.cpp

clean:
	$(RM) $(OBJS0)

dist-clean: clean
	$(RM) $(EXEC)
