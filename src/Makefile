CXX = g++
CFLAGS = -Ofast -march=native -mtune=native -fomit-frame-pointer -Wall 
ODIR = obj
SRC = bitboard.cpp board.cpp main.cpp movegenerator.cpp search.cpp uci.cpp test.cpp eval.cpp tt.cpp
OBJ = $(patsubst %.cpp,$(ODIR)/%.o,$(SRC))

BiggerBotChess: $(OBJ)
	$(CXX) -o $@ $^ $(CFLAGS)

$(ODIR)/%.o: %.cpp
	$(CXX) -c $< -o $@ $(CFLAGS)

clean:
	rm -f $(ODIR)/*.o BiggerBotChess