#pragma once

#include "UltimateMainBoard.h"
#include <cstdlib>
#include <ctime>
#include <vector>
class Ultimate_Random {
public:
    Ultimate_Random(char symbol) : symbol(symbol) {
        std::srand(std::time(0));
    }

    std::pair<int, int> get_move(UltimateMainBoard* game) {
        std::vector<std::pair<int, int>> available_moves;
        for (int row = 0; row < 9; ++row) {
            for (int col = 0; col < 9; ++col) {
                if (game->get_cell_value(row, col) == '.') {
                    available_moves.push_back({ row, col });
                }
            }
        }
        if (!available_moves.empty()) {
            int index = std::rand() % available_moves.size();
            return available_moves[index];
        }
        return { -1, -1 }; // No valid moves
    }

private:
    char symbol;
};