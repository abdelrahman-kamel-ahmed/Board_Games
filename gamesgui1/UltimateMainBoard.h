#pragma once

#include <iostream>
#include <array>
#include <vector>
#include <limits>
#include <algorithm>
#include <cstdlib>
#include "MinMaxPlayer.h"
#include "BoardGame_Classes.h"

#undef max
#undef min

class Sub3x3TicTacToeBoard {
public:
    Sub3x3TicTacToeBoard() {
        for (auto& row : board) {
            row.fill('.');
        }
    }

    bool update_board(int row, int col, char player) {
        if (row >= 0 && row < 3 && col >= 0 && col < 3 && board[row][col] == '.') {
            board[row][col] = player;
            return true;
        }
        return false;
    }

    bool is_win() {
        for (int i = 0; i < 3; ++i) {
            if (board[i][0] != '.' && board[i][0] == board[i][1] && board[i][1] == board[i][2])
                return true;
            if (board[0][i] != '.' && board[0][i] == board[1][i] && board[1][i] == board[2][i])
                return true;
        }
        if (board[0][0] != '.' && board[0][0] == board[1][1] && board[1][1] == board[2][2])
            return true;
        if (board[0][2] != '.' && board[0][2] == board[1][1] && board[1][1] == board[2][0])
            return true;
        return false;
    }

    bool is_draw() {
        if (is_win()) return false;
        for (const auto& row : board) {
            for (char cell : row) {
                if (cell == '.') return false;
            }
        }
        return true;
    }

    char get_cell_value(int row, int col) {
        return board[row][col];
    }

private:
    std::array<std::array<char, 3>, 3> board;
};

class UltimateMainBoard {
public:
    UltimateMainBoard() {
        for (auto& row : small_boards) {
            for (auto& board : row) {
                board = new Sub3x3TicTacToeBoard();
            }
        }
        for (auto& row : big_board) {
            row.fill('.');
        }
    }

    ~UltimateMainBoard() {
        for (auto& row : small_boards) {
            for (auto& board : row) {
                delete board;
            }
        }
    }

    bool update_board(int row, int col, char player) {
        int big_row = row / 3;
        int big_col = col / 3;
        int small_row = row % 3;
        int small_col = col % 3;

        if (big_board[big_row][big_col] == '.' && small_boards[big_row][big_col]->update_board(small_row, small_col, player)) {
            if (small_boards[big_row][big_col]->is_win()) {
                big_board[big_row][big_col] = player;
            }
            return true;
        }
        return false;
    }

    bool is_win() {
        for (int i = 0; i < 3; ++i) {
            if (big_board[i][0] != '.' && big_board[i][0] == big_board[i][1] && big_board[i][1] == big_board[i][2])
                return true;
            if (big_board[0][i] != '.' && big_board[0][i] == big_board[1][i] && big_board[1][i] == big_board[2][i])
                return true;
        }
        if (big_board[0][0] != '.' && big_board[0][0] == big_board[1][1] && big_board[1][1] == big_board[2][2])
            return true;
        if (big_board[0][2] != '.' && big_board[0][2] == big_board[1][1] && big_board[1][1] == big_board[2][0])
            return true;
        return false;
    }

    bool is_draw() {
        if (is_win()) return false;
        for (int big_row = 0; big_row < 3; ++big_row) {
            for (int big_col = 0; big_col < 3; ++big_col) {
                if (big_board[big_row][big_col] == '.') {
                    if (!small_boards[big_row][big_col]->is_draw()) {
                        return false;
                    }
                }
            }
        }
        return true;
    }

    bool game_is_over() {
        return is_win() || is_draw();
    }

    char get_cell_value(int row, int col) {
        int big_row = row / 3;
        int big_col = col / 3;
        int small_row = row % 3;
        int small_col = col % 3;
        return small_boards[big_row][big_col]->get_cell_value(small_row, small_col);
    }

    void undo_move(int row, int col) {
        int big_row = row / 3;
        int big_col = col / 3;
        int small_row = row % 3;
        int small_col = col % 3;
        small_boards[big_row][big_col]->update_board(small_row, small_col, '.');
    }

private:
    std::array<std::array<Sub3x3TicTacToeBoard*, 3>, 3> small_boards;
    std::array<std::array<char, 3>, 3> big_board;
};

template <typename T>
int get_move(RandomPlayer<T>* player, UltimateMainBoard& board) {
    std::vector<int> available_moves;
    for (int row = 0; row < 9; ++row) {
        for (int col = 0; col < 9; ++col) {
            if (board.get_cell_value(row, col) == '.') {
                available_moves.push_back(row * 9 + col);
            }
        }
    }
    if (available_moves.empty()) return -1;
    int move = available_moves[rand() % available_moves.size()];
    return move;
}

template <typename T>
int get_move(X_O_MinMax_Player<T>* player, UltimateMainBoard& board) {
    int bestMove = -1;
    int bestValue = std::numeric_limits<int>::lowest();
    T symbol = player->getsymbol();
    T opponentSymbol = (symbol == 'X') ? 'O' : 'X';

    for (int row = 0; row < 9; ++row) {
        for (int col = 0; col < 9; ++col) {
            if (board.update_board(row, col, symbol)) {
                int moveValue = minmax(board, 0, false, symbol);
                board.undo_move(row, col);
                if (moveValue > bestValue) {
                    bestMove = row * 9 + col;
                    bestValue = (std::max)(bestValue, moveValue);
                }
            }
        }
    }
    return bestMove;
}

template <typename T>
int minmax(UltimateMainBoard& board, int depth, bool isMaximizing, T symbol) {
    const int MAX_DEPTH = 5;

    if (depth == MAX_DEPTH || board.is_win() || board.is_draw()) {
        return evaluate_board(board, symbol);
    }

    T opponentSymbol = (symbol == 'X') ? 'O' : 'X';

    if (isMaximizing) {
        int bestValue = std::numeric_limits<int>::lowest();
        for (int row = 0; row < 9; ++row) {
            for (int col = 0; col < 9; ++col) {
                if (board.update_board(row, col, symbol)) {
                    int moveValue = minmax(board, depth + 1, false, symbol);
                    board.undo_move(row, col);
                    bestValue = (std::max)(bestValue, moveValue);
                }
            }
        }
        return bestValue;
    }
    else {
        int bestValue = std::numeric_limits<int>::max();
        for (int row = 0; row < 9; ++row) {
            for (int col = 0; col < 9; ++col) {
                if (board.update_board(row, col, opponentSymbol)) {
                    int moveValue = minmax(board, depth + 1, true, symbol);
                    board.undo_move(row, col);
                    bestValue = (std::min)(bestValue, moveValue);
                }
            }
        }
        return bestValue;
    }
}

template <typename T>
int evaluate_board(UltimateMainBoard& board, T symbol) {
    const int directions[4][2] = { {0, 1}, {1, 0}, {1, 1}, {1, -1} };
    int score = 0;

    for (int row = 0; row < 9; ++row) {
        for (int col = 0; col < 9; ++col) {
            char current = board.get_cell_value(row, col);
            if (current == '.') continue;

            for (auto& dir : directions) {
                int r = row, c = col, count = 0, openEnds = 0;

                while (r >= 0 && r < 9 && c >= 0 && c < 9 && board.get_cell_value(r, c) == current) {
                    count++;
                    r += dir[0];
                    c += dir[1];
                }

                if (r >= 0 && r < 9 && c >= 0 && c < 9 && board.get_cell_value(r, c) == '.') {
                    openEnds++;
                }

                r = row - dir[0];
                c = col - dir[1];
                if (r >= 0 && r < 9 && c >= 0 && c < 9 && board.get_cell_value(r, c) == '.') {
                    openEnds++;
                }

                if (count == 4) {
                    score += (current == symbol) ? 1000 : -1000;
                }
                else if (count == 3 && openEnds > 0) {
                    score += (current == symbol) ? 50 : -50;
                }
                else if (count == 2 && openEnds > 1) {
                    score += (current == symbol) ? 10 : -10;
                }
            }
        }
    }

    return score;
}
