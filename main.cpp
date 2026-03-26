#include <stdio.h>
#include <iostream>
#include "types.h"
#include "boardClass.h"




int main () {
	Board board;
	board.MakeMove(0);
	board.MakeMove(62);
	board.MakeMove(62);
	board.MakeMove(63);
	board.MakeMove(64);
	board.MakeMove(80);
	board.MakeMove(81);
	board.MakeMove(100);

}

//void makeMove(Board board, int piece, int squareIndex) {
//	// check if squareIndex is open (check if the move is legal)
//
//	if (piece == (board.turn % 2)) { // check if X makes move
//		// change xBoard1/2 and openSpots1/2
//		// log the move
//	}
//	else {
//
//		//log the move
//	}
//	board.turn += 1; // increment the turn by one
//	// mark what box to move in/give free turn
//
//}
//
//class Log {
//public:
//	const int LogLevelError = 0;
//	const int LogLevelWarning = 1;
//	const int LogLevelInfo = 2;
//private:
//	int m_LogLevel = LogLevelInfo;
//public:
//	void SetLevel(int level) {
//		m_LogLevel = level;
//	}
//	void Error(const char* message) {
//		if (m_LogLevel >= LogLevelError)
//			std::cout << "[ERROR]" << message << std::endl;
//	}
//	void Warn(const char* message) {
//		if (m_LogLevel >= LogLevelWarning)
//			std::cout << "[WARNING]" << message << std::endl;
//	}
//	void Info(const char* message) {
//		if (m_LogLevel >= LogLevelInfo)
//			std::cout << "[Info]" << message << std::endl;
//	}
//};
