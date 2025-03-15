#pragma once
#include "BoardGame_Classes.h"
#include <set>
#include <vector>
#include <iostream>

namespace gamesgui {

    using namespace System;
    using namespace System::Windows::Forms;
    using namespace System::Drawing;

    // Template class for Numerical Tic-Tac-Toe board
    template <typename T>
    class NumericalTicTacToeBoard : public Board<T> {
    private:
        std::vector<std::vector<T>> board;
        std::set<T> used_numbers;

    public:
        int rows, columns, n_moves;

        NumericalTicTacToeBoard() : rows(3), columns(3), n_moves(0) {
            board.resize(rows, std::vector<T>(columns, 0));
        }
        const std::set<T>& get_used_numbers() const {
            return used_numbers;
        }
        bool update_board(int x, int y, T number) override {
            if (x < 0 || x >= columns || y < 0 || y >= rows) return false;
            if (board[y][x] != 0 || used_numbers.count(number) > 0) return false;

            board[y][x] = number;
            used_numbers.insert(number);
            n_moves++;
            return true;
        }

        void display_board() override {
            for (const auto& row : board) {
                for (const auto& cell : row) {
                    std::cout << cell << " ";
                }
                std::cout << std::endl;
            }
        }

        bool is_win() override {
            // Check rows and columns
            for (int i = 0; i < rows; ++i) {
                if (board[i][0] + board[i][1] + board[i][2] == 15) return true;
                if (board[0][i] + board[1][i] + board[2][i] == 15) return true;
            }
            // Check diagonals
            if (board[0][0] + board[1][1] + board[2][2] == 15) return true;
            if (board[0][2] + board[1][1] + board[2][0] == 15) return true;

            return false;
        }

        bool is_draw() override {
            return n_moves == rows * columns && !is_win();
        }

        bool game_is_over() override {
            return is_win() || is_draw();
        }

        void reset_board() {
            board.assign(rows, std::vector<T>(columns, 0));
            used_numbers.clear();
            n_moves = 0;
        }
    };

    // Numerical Tic-Tac-Toe GUI class
    public ref class NumericalTicTacToe : public Form {
    private:
        NumericalTicTacToeBoard<int>* board;
        TableLayoutPanel^ tableLayoutPanel;
        Button^ confirmButton;
        bool isPlayer1Turn = true;
        int currentNumber = 0;

    public:
        NumericalTicTacToe() {
            InitializeComponent();
            // Set the background color of the form
            this->BackColor = System::Drawing::Color::LightBlue;

            // Create the title label
            Label^ titleLabel = gcnew Label();
            titleLabel->Text = L"Numerical Tic-Tac-Toe";
            titleLabel->Font = gcnew System::Drawing::Font(L"Palatino Linotype", 16.0F, System::Drawing::FontStyle::Bold, System::Drawing::GraphicsUnit::Point);
            titleLabel->BackColor = System::Drawing::Color::LightBlue;
            titleLabel->AutoSize = false;
            titleLabel->TextAlign = System::Drawing::ContentAlignment::MiddleCenter;
            titleLabel->Dock = System::Windows::Forms::DockStyle::Top;
            titleLabel->Height = 50; // Fixed height for the title area
            this->Controls->Add(titleLabel);
            // Initialize game board
            board = new NumericalTicTacToeBoard<int>();

            // Set up TableLayoutPanel
            tableLayoutPanel = gcnew TableLayoutPanel();
            tableLayoutPanel->RowCount = board->rows;
            tableLayoutPanel->ColumnCount = board->columns;
            tableLayoutPanel->Dock = DockStyle::None;
            tableLayoutPanel->CellBorderStyle = TableLayoutPanelCellBorderStyle::Single;
            tableLayoutPanel->Location = Point((this->ClientSize.Width - tableLayoutPanel->Width) / 2, (this->ClientSize.Height - tableLayoutPanel->Height) / 2);

            // Set uniform row and column sizes
            for (int i = 0; i < board->rows; ++i) {
                tableLayoutPanel->RowStyles->Add(gcnew RowStyle(SizeType::Percent, 100.0F / board->rows));
            }
            for (int j = 0; j < board->columns; ++j) {
                tableLayoutPanel->ColumnStyles->Add(gcnew ColumnStyle(SizeType::Percent, 100.0F / board->columns));
            }

            // Add buttons to the grid
            for (int i = 0; i < board->rows; ++i) {
                for (int j = 0; j < board->columns; ++j) {
                    Button^ button = CreateGridButton(i, j);
                    tableLayoutPanel->Controls->Add(button, j, i);
                }
            }

            // Add Confirm button
            confirmButton = gcnew Button();
            confirmButton->Text = "Confirm";
            confirmButton->Dock = DockStyle::Bottom;
            confirmButton->Height = 40;
            confirmButton->Font = gcnew System::Drawing::Font(L"Arial", 12.0F, System::Drawing::FontStyle::Bold);
            confirmButton->BackColor = System::Drawing::Color::LightGreen;
            confirmButton->ForeColor = System::Drawing::Color::DarkGreen;
            confirmButton->FlatStyle = FlatStyle::Flat;
            confirmButton->FlatAppearance->BorderSize = 0;
            confirmButton->Click += gcnew EventHandler(this, &NumericalTicTacToe::OnConfirmClick);

            // Add components to form
            this->Controls->Add(tableLayoutPanel);
            this->Controls->Add(confirmButton);
        }

    protected:
        ~NumericalTicTacToe() {
            delete board;
        }

    private:
        void InitializeComponent() {
            this->Size = Drawing::Size(400, 400);
            this->Text = "Numerical Tic-Tac-Toe";
            this->Padding = System::Windows::Forms::Padding(10);
            this->BackColor = Color::LightBlue;
        }

        Button^ CreateGridButton(int row, int col) {
            Button^ button = gcnew Button();
            button->Dock = DockStyle::Fill;
            button->Text = L"0";
            button->Font = gcnew Drawing::Font("Arial", 14, FontStyle::Bold);
            button->Tag = gcnew Drawing::Point(row, col);
            button->BackColor = Color::White;
            button->Click += gcnew EventHandler(this, &NumericalTicTacToe::OnCellClick);
            return button;
        }

        void OnCellClick(Object^ sender, EventArgs^ e) {
            Button^ button = safe_cast<Button^>(sender);
            int number = Int32::Parse(button->Text);
            number = (number + 1) % 10; // Increment number and wrap around after 9
            button->Text = number.ToString();
        }

        void OnConfirmClick(Object^ sender, EventArgs^ e) {
            for each(Control ^ control in tableLayoutPanel->Controls) {
                Button^ button = dynamic_cast<Button^>(control);
                if (button && button->Text != "0") {
                    Point cell = safe_cast<Point>(button->Tag);
                    int x = cell.Y, y = cell.X;
                    int number = Int32::Parse(button->Text);

                    if (board->update_board(x, y, number)) {
                        button->ForeColor = isPlayer1Turn ? Color::Red : Color::Blue;

                        if (board->is_win()) {
                            MessageBox::Show(isPlayer1Turn ? "Player 1 wins!" : "Player 2 wins!", "Game Over");
                            ResetGame();
                            return;
                        }
                        else if (board->is_draw()) {
                            MessageBox::Show("It's a draw!", "Game Over");
                            ResetGame();
                            return;
                        }
                        else {
                            isPlayer1Turn = !isPlayer1Turn;
                        }
                    }
                }
            }
        }

        void ResetGame() {
            board->reset_board();
            for each(Control ^ control in tableLayoutPanel->Controls) {
                Button^ button = dynamic_cast<Button^>(control);
                if (button) button->Text = "0";
            }
            isPlayer1Turn = true;
        }
    };
}