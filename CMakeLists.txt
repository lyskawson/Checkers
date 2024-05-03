cmake_minimum_required(VERSION 3.24)
project(Checkers)

set(CMAKE_CXX_STANDARD 17)

add_executable(Checkers main.cpp CheckersBoard.cpp CheckersBoard.h Array2D.h Controller.cpp Controller.h HumanPlayer.cpp HumanPlayer.h Player.h ComputerPlayer.cpp ComputerPlayer.h)
