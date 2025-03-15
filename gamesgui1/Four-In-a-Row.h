#pragma once
#include "BoardGame_Classes.h"
#include "MinMaxPlayer.h"
#include <limits>
#include <algorithm>
#include <vector>
#include <iostream>
#include <cstdlib> // for rand()
#include<cmath>
namespace gamesgui {

    using namespace System;
    using namespace System::ComponentModel;
    using namespace System::Collections;
    using namespace System::Windows::Forms;
    using namespace System::Data;
    using namespace System::Drawing;

    template <typename T>
    class FourInARowBoard : public Board<T> {
    private:
        std::vector<std::vector<T>> board;

    public:
        int rows;
        int columns;
        int n_moves;

        FourInARowBoard() { // 6x7 board 
            this->rows = 6;
            this->columns = 7;
            this->board.resize(this->rows, std::vector<T>(this->columns, 0));
            this->n_moves = 0;
        }

        bool update_board(int x, int y, T symbol) override {
            // bounds of x column
            if (x < 0 || x >= this->columns) return false;

            // Find the lowest empty row in the column
            for (int row = this->rows - 1; row >= 0; --row)
            {
                if (this->board[row][x] == 0)
                {
                    this->board[row][x] = symbol;
                    this->n_moves++;
                    return true;
                }
            }
            return false; // Column is full
        }

        void display_board() override
        {
            for (int i = 0; i < this->rows; ++i)
            {
                for (int j = 0; j < this->columns; ++j)
                {
                    std::cout << this->board[i][j] << " ";
                }
                std::cout << std::endl;
            }
        }

        bool is_win() override {
            const int directions[4][2] = { {0, 1}, {1, 0}, {1, 1}, {1, -1} };

            for (int row = 0; row < this->rows; ++row)
            {
                for (int col = 0; col < this->columns; ++col)
                {
                    T current = this->board[row][col];
                    if (current == 0) continue;

                    for (auto& dir : directions)
                    {
                        int r = row, c = col, count = 0;

                        while (r >= 0 && r < this->rows && c >= 0 && c < this->columns &&
                            this->board[r][c] == current)
                        {
                            count++;
                            if (count == 4) return true;
                            r += dir[0];
                            c += dir[1];
                        }
                    }
                }
            }
            return false;
        }

        bool is_draw() override
        {
            return this->n_moves == this->rows * this->columns;
        }

        bool game_is_over() override
        {
            return is_win() || is_draw();
        }

        T get_cell_value(int row, int col) const {
            return this->board[row][col];
        }

        void undo_move(int col) {
            for (int row = 0; row < this->rows; ++row) {
                if (this->board[row][col] != 0) {
                    this->board[row][col] = 0;
                    this->n_moves--;
                    return;
                }
            }
        }
    };

   // Define get_move method for RandomPlayer
    template <typename T>
    int get_move(RandomPlayer<T>* player, FourInARowBoard<T>& board) {
        int column;
        std::vector<int> available_columns;
        for (int col = 0; col < board.columns; ++col) {
            if (board.get_cell_value(0, col) == 0) {
                available_columns.push_back(col);
            }
        }
        if (available_columns.empty()) return -1; // No valid moves
        column = available_columns[rand() % available_columns.size()];
        return column;
    }

    // Define get_move method for X_O_MinMax_Player
    template <typename T>
    int get_move(X_O_MinMax_Player<T>* player, FourInARowBoard<T>& board) {
        int bestMove = -1;
        int bestValue = std::numeric_limits<int>::lowest();
        T symbol = player->getsymbol();
        T opponentSymbol = (symbol == 1) ? 2 : 1;

        // Check for immediate winning move
        for (int col = 0; col < board.columns; ++col) {
            if (board.update_board(col, 0, symbol)) {
                if (board.is_win()) {
                    board.undo_move(col);
                    return col; // Return the winning move immediately
                }
                board.undo_move(col);
            }
        }

        // Check for immediate opponent winning move and block it
        for (int col = 0; col < board.columns; ++col) {
            if (board.update_board(col, 0, opponentSymbol)) {
                if (board.is_win()) {
                    board.undo_move(col);
                    return col; // Block the opponent's winning move
                }
                board.undo_move(col);
            }
        }

        // If no immediate winning or blocking move, proceed with MinMax
        for (int col = 0; col < board.columns; ++col) {
            if (board.update_board(col, 0, symbol)) {
                int moveValue = minmax(board, 0, false, symbol);
                board.undo_move(col); // Undo move

                if (moveValue > bestValue) {
                    bestMove = col;
                    bestValue = moveValue;
                }
            }
        }
        return bestMove;
    }

    // MinMax algorithm for X_O_MinMax_Player
    template <typename T>
    int minmax(FourInARowBoard<T>& board, int depth, bool isMaximizing, T symbol) {
        const int MAX_DEPTH = 5; // Limit search depth

        if (depth == MAX_DEPTH || board.is_win() || board.is_draw()) {
            return evaluate_board(board, symbol); // Add heuristic evaluation
        }

        T opponentSymbol = (symbol == 1) ? 2 : 1;

        if (isMaximizing) {
            int bestValue = std::numeric_limits<int>::lowest();

            for (int col = 0; col < board.columns; ++col) {
                if (board.update_board(col, 0, symbol)) {
                    if (board.is_win()) {
                        board.undo_move(col);
                        return 1000; // High positive value for a win
                    }
                    int value = minmax(board, depth + 1, false, symbol);
                    board.undo_move(col); // Undo move
                    bestValue = std::max(bestValue, value);
                }
            }
            return bestValue;
        }
        else {
            int bestValue = std::numeric_limits<int>::max();
            for (int col = 0; col < board.columns; ++col) {
                if (board.update_board(col, 0, opponentSymbol)) {
                    if (board.is_win()) {
                        board.undo_move(col);
                        return -1000; // High negative value for a loss
                    }
                    int value = minmax(board, depth + 1, true, symbol);
                    board.undo_move(col); // Undo move
                    bestValue = std::min(bestValue, value);
                }
            }
            return bestValue;
        }
    }

    // Evaluate the board for the given symbol
    template <typename T>
    int evaluate_board(FourInARowBoard<T>& board, T symbol) {
        const int directions[4][2] = { {0, 1}, {1, 0}, {1, 1}, {1, -1} };
        int score = 0;

        for (int row = 0; row < board.rows; ++row) {
            for (int col = 0; col < board.columns; ++col) {
                T current = board.get_cell_value(row, col);
                if (current == 0) continue;

                for (auto& dir : directions) {
                    int r = row, c = col, count = 0, openEnds = 0;

                    // Count consecutive pieces and check for open ends
                    while (r >= 0 && r < board.rows && c >= 0 && c < board.columns &&
                        board.get_cell_value(r, c) == current) {
                        count++;
                        r += dir[0];
                        c += dir[1];
                    }

                    // Check if the sequence is open-ended
                    if (r >= 0 && r < board.rows && c >= 0 && c < board.columns &&
                        board.get_cell_value(r, c) == 0) {
                        openEnds++;
                    }

                    r = row - dir[0];
                    c = col - dir[1];
                    if (r >= 0 && r < board.rows && c >= 0 && c < board.columns &&
                        board.get_cell_value(r, c) == 0) {
                        openEnds++;
                    }

                    // Assign scores based on patterns
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

    public ref class FourInaRow : public System::Windows::Forms::Form
    {
    private:
        FourInARowBoard<int>* board;
        System::Windows::Forms::TableLayoutPanel^ tableLayoutPanel;
        Player<int>* player1;
        Player<int>* player2;
        bool isPlayer1Turn = true; // For taking turns

    public:
        FourInaRow(Player<int>* p1, Player<int>* p2)
        {
            InitializeComponent();

            // For color of the form
            this->BackColor = System::Drawing::Color::FromArgb(0x2f, 0x4f, 0x4f);

            // Title label
            Label^ titleLabel = gcnew Label();
            titleLabel->Text = L"Four In a Row";
            titleLabel->Font = gcnew System::Drawing::Font(L"Palatino Linotype", 16.0F, System::Drawing::FontStyle::Bold, System::Drawing::GraphicsUnit::Point);
            titleLabel->BackColor = System::Drawing::Color::FromArgb(0x2f, 0x4f, 0x4f);
            titleLabel->AutoSize = false;
            titleLabel->TextAlign = System::Drawing::ContentAlignment::MiddleCenter;
            titleLabel->Dock = System::Windows::Forms::DockStyle::Top;
            titleLabel->Height = 50;
            this->Controls->Add(titleLabel);

            board = new FourInARowBoard<int>();
            player1 = p1;
            player2 = p2;

            // TableLayoutPanel
            tableLayoutPanel = gcnew System::Windows::Forms::TableLayoutPanel();
            tableLayoutPanel->RowCount = board->rows;
            tableLayoutPanel->ColumnCount = board->columns;
            tableLayoutPanel->Dock = System::Windows::Forms::DockStyle::None;
            tableLayoutPanel->Size = System::Drawing::Size(400, 300);
            tableLayoutPanel->Location = System::Drawing::Point((this->Width - tableLayoutPanel->Width) / 2, 100);
            tableLayoutPanel->CellBorderStyle = System::Windows::Forms::TableLayoutPanelCellBorderStyle::Single;

            // Uniform Row and Column sizes
            for (int i = 0; i < board->rows; ++i)
            {
                tableLayoutPanel->RowStyles->Add(gcnew System::Windows::Forms::RowStyle(System::Windows::Forms::SizeType::Percent, 100.0F / board->rows));
            }
            for (int j = 0; j < board->columns; ++j)
            {
                tableLayoutPanel->ColumnStyles->Add(gcnew System::Windows::Forms::ColumnStyle(System::Windows::Forms::SizeType::Percent, 100.0F / board->columns));
            }

            // buttons of the grid
            for (int i = 0; i < board->rows; ++i)
            {
                for (int j = 0; j < board->columns; ++j)
                {
                    Button^ button = gcnew Button();
                    button->Dock = System::Windows::Forms::DockStyle::Fill;
                    button->Text = L"";
                    button->Tag = gcnew System::Drawing::Point(i, j);
                    button->BackColor = System::Drawing::Color::White;
                    button->FlatStyle = System::Windows::Forms::FlatStyle::Flat;
                    button->FlatAppearance->BorderSize = 0;
                    button->Click += gcnew System::EventHandler(this, &FourInaRow::OnCellClick);
                    tableLayoutPanel->Controls->Add(button, j, i);
                }
            }

            this->Controls->Add(tableLayoutPanel);

            // If player 1 is AI, make the first move
            if (dynamic_cast<RandomPlayer<int>*>(player1) || dynamic_cast<X_O_MinMax_Player<int>*>(player1)) {
                startAITimer();
            }
        }

    protected:
        ~FourInaRow()
        {
            if (components)
            {
                delete components;
            }
            delete board;
            delete player1;
            delete player2;
        }

    private:
        System::ComponentModel::Container^ components;

#pragma region Windows Form Designer generated code
        void InitializeComponent(void)
        {
            this->components = gcnew System::ComponentModel::Container();
            this->Size = System::Drawing::Size(500, 500);
            this->Text = L"Four In a Row";
            this->Padding = System::Windows::Forms::Padding(10);
            this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
        }
#pragma endregion
    private:
        System::Windows::Forms::Timer^ aiTimer;

        void startAITimer() {
            aiTimer = gcnew System::Windows::Forms::Timer();
            aiTimer->Interval = 500; 
            aiTimer->Tick += gcnew System::EventHandler(this, &FourInaRow::onAITimerTick);
            aiTimer->Start();
        }

        void onAITimerTick(System::Object^ sender, System::EventArgs^ e) {
            if (board->game_is_over()) {
                aiTimer->Stop();
                return;
            }
            makeAIMove();
        }

    private:
        void makeAIMove() {
            int column;
            Player<int>* currentPlayer = isPlayer1Turn ? player1 : player2;

            if (dynamic_cast<RandomPlayer<int>*>(currentPlayer)) {
                column = get_move<int>(dynamic_cast<RandomPlayer<int>*>(currentPlayer), *board);
            }
            else if (dynamic_cast<X_O_MinMax_Player<int>*>(currentPlayer)) {
                column = get_move<int>(dynamic_cast<X_O_MinMax_Player<int>*>(currentPlayer), *board);
            }

            if (column != -1 && board->update_board(column, 0, isPlayer1Turn ? 1 : 2)) {
                makeMoveOnUI(column, isPlayer1Turn ? 1 : 2);
               
                if (board->is_win()) {
                    showMessage(isPlayer1Turn ? "Player 1 wins!" : "Player 2 wins!");
                    reset_game();
                    return;
                }
                else if (board->is_draw()) {
                    showMessage("It's a draw!");
                    reset_game();
                    return;
                }
                else {
                    isPlayer1Turn = !isPlayer1Turn;
                }
            }
        }

        void showMessage(String^ message) {
            MessageBox::Show(message, "Game Over");
        }

    private:
        void OnCellClick(System::Object^ sender, System::EventArgs^ e) {
            Button^ clickedButton = dynamic_cast<Button^>(sender);
            System::Drawing::Point^ position = dynamic_cast<System::Drawing::Point^>(clickedButton->Tag);
            int column = position->Y;

            if (isPlayer1Turn) {
                if (board->update_board(column, 0, 1)) {
                    makeMoveOnUI(column, 1);
                    if (board->is_win()) {
                        MessageBox::Show("Player 1 wins!", "Game Over");
                        reset_game();
                    }
                    else if (board->is_draw()) {
                        MessageBox::Show("It's a draw!", "Game Over");
                        reset_game();
                    }
                    else {
                        isPlayer1Turn = !isPlayer1Turn;
                        if (dynamic_cast<RandomPlayer<int>*>(player2) || dynamic_cast<X_O_MinMax_Player<int>*>(player2)) {
                            startAITimer();
                        }
                    }
                }
            }
            else {
                if (board->update_board(column, 0, 2)) {
                    makeMoveOnUI(column, 2);
                    if (board->is_win()) {
                        MessageBox::Show("Player 2 wins!", "Game Over");
                        reset_game();
                    }
                    else if (board->is_draw()) {
                        MessageBox::Show("It's a draw!", "Game Over");
                        reset_game();
                    }
                    else {
                        isPlayer1Turn = !isPlayer1Turn;
                        if (dynamic_cast<RandomPlayer<int>*>(player1) || dynamic_cast<X_O_MinMax_Player<int>*>(player1)) {
                            startAITimer();
                        }
                    }
                }
            }
        }

        void makeMoveOnUI(int column, int symbol) {
            for (int i = board->rows - 1; i >= 0; --i) {
                Button^ cell = safe_cast<Button^>(tableLayoutPanel->GetControlFromPosition(column, i));
                if (cell->Text == "") {
                    cell->Text = (symbol == 1) ? "X" : "O";
                    cell->ForeColor = (symbol == 1) ? System::Drawing::Color::Red : System::Drawing::Color::Blue;
                    break;
                }
            }
        }

        void reset_game()
        {
            delete board;
            board = new FourInARowBoard<int>();
            for each(Control ^ control in tableLayoutPanel->Controls)
            {
                Button^ button = dynamic_cast<Button^>(control);
                if (button) button->Text = "";
            }

            isPlayer1Turn = true;
        }
    };

}

