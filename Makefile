all: StudentBoard.cpp StudentGame.cpp
	clang++ --std=c++11 StudentBoard.cpp StudentGame.cpp

clean:
	rm a.out