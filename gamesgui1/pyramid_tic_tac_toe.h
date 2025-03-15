#pragma once
#include "BoardGame_Classes.h"
#include <cstdlib>
#include <ctime>

namespace gamesgui {

    using namespace System;
    using namespace System::ComponentModel;
    using namespace System::Collections;
    using namespace System::Windows::Forms;
    using namespace System::Data;
    using namespace System::Drawing;

    template <typename T>
    class PyramidBoard : public Board<T> {
    private:
        std::vector<std::vector<T>> board;

    public:
        int rows;
        int columns;
        int n_moves;

        PyramidBoard() {
            this->rows = 3;
            this->columns = 5; // Max columns in the third row
            this->board = {
                { ' ', ' ', ' ', ' ', ' ' },
                { ' ', ' ', ' ', ' ', ' ' },
                { ' ', ' ', ' ', ' ', ' ' }
            };
            this->n_moves = 0;
        }

        bool update_board(int x, int y, T symbol) override {
            if (x < 0 || x >= this->columns || y < 0 || y >= this->rows) return false;
            if (this->board[y][x] != ' ') return false;

            this->board[y][x] = symbol;
            this->n_moves++;
            return true;
        }

        void display_board() override {
            for (int i = 0; i < this->rows; ++i) {
                for (int j = 0; j < this->columns; ++j) {
                    std::cout << this->board[i][j] << " ";
                }
                std::cout << std::endl;
            }
        }

        bool is_win() override {
            const int directions[4][2] = { {0, 1}, {1, 0}, {1, 1}, {1, -1} };
            for (int row = 0; row < this->rows; ++row) {
                for (int col = 0; col < this->columns; ++col) {
                    T current = this->board[row][col];
                    if (current == ' ') continue;

                    for (auto& dir : directions) {
                        int r = row, c = col, count = 0;
                        while (r >= 0 && r < this->rows && c >= 0 && c < this->columns &&
                            this->board[r][c] == current) {
                            count++;
                            if (count == 3) return true;
                            r += dir[0];
                            c += dir[1];
                        }
                    }
                }
            }
            return false;
        }

        bool is_draw() override {
            // Check if all positions are filled without a win
            for (int row = 0; row < this->rows; ++row) {
                for (int col = 0; col < this->columns; ++col) {
                    // Skip invalid positions on the pyramid shape (e.g., empty spaces in row 1 and 2)
                    if ((row == 0 && col != 2) || (row == 1 && (col == 0 || col == 4))) {
                        continue;
                    }
                    if (this->board[row][col] == ' ') {
                        return false; // Found an empty spot, so no draw
                    }
                }
            }
            // If the board is full and there's no win, it's a draw
            return !is_win();
        }

        bool game_is_over() override {
            return is_win() || is_draw();
        }
    };

    template <typename T>
    class PyramidRandomPlayer : public RandomPlayer<T> {
    public:
        PyramidRandomPlayer(T symbol) : RandomPlayer<T>(symbol) {
            srand(static_cast<unsigned int>(time(0)));  // Seed the random number generator
        }

        void getmove(int& x, int& y) override {
            do {
                x = rand() % 5;
                y = rand() % 3;
            } while ((y == 0 && x != 2) || (y == 1 && (x == 0 || x == 4))); // Ensure move is within valid pyramid positions
        }
    };

    public ref class ManagedPyramidTicTacToe : public System::Windows::Forms::Form
    {
    private:
        PyramidBoard<char>* board;
        PyramidRandomPlayer<char>* computerPlayer;
        System::Windows::Forms::TableLayoutPanel^ tableLayoutPanel;
        bool isXturn = true; // Track whose turn it is
        bool isHumanVsComputer; // Track if the game is human vs computer

    public:
        ManagedPyramidTicTacToe(bool humanVsComputer)
        {
            InitializeComponent();

            // Set the background color of the form
            this->BackColor = System::Drawing::Color::LightBlue;

            // Create the title label
            Label^ titleLabel = gcnew Label();
            titleLabel->Text = L"Pyramid Tic-Tac-Toe";
            titleLabel->Font = gcnew System::Drawing::Font(L"Playbill", 35, System::Drawing::FontStyle::Bold, System::Drawing::GraphicsUnit::Point);
            titleLabel->BackColor = System::Drawing::Color::LightBlue;
            titleLabel->AutoSize = false;
            titleLabel->TextAlign = System::Drawing::ContentAlignment::MiddleCenter;
            titleLabel->Dock = System::Windows::Forms::DockStyle::Top;
            titleLabel->Height = 50; // Fixed height for the title area
            this->Controls->Add(titleLabel);

            board = new PyramidBoard<char>();
            computerPlayer = new PyramidRandomPlayer<char>('O');
            isHumanVsComputer = humanVsComputer;

            // Initialize the TableLayoutPanel
            tableLayoutPanel = gcnew System::Windows::Forms::TableLayoutPanel();
            tableLayoutPanel->Dock = System::Windows::Forms::DockStyle::None;
            tableLayoutPanel->Size = System::Drawing::Size(400, 300); // Fixed size for the grid
            tableLayoutPanel->Location = System::Drawing::Point((this->Width - tableLayoutPanel->Width) / 2, 100); // Center the grid
            tableLayoutPanel->CellBorderStyle = System::Windows::Forms::TableLayoutPanelCellBorderStyle::None;
            tableLayoutPanel->Margin = System::Windows::Forms::Padding(0);
            tableLayoutPanel->Padding = System::Windows::Forms::Padding(0);
            // Define the number of columns
            tableLayoutPanel->ColumnCount = 5; // Max number of columns (5 in the last row)

            // Define the row styles for each row
            tableLayoutPanel->RowCount = 3;

            // Row 1: 1 cell
            tableLayoutPanel->RowStyles->Add(gcnew System::Windows::Forms::RowStyle(System::Windows::Forms::SizeType::Percent, 33.33F));
            Button^ button1 = gcnew Button();
            button1->Dock = System::Windows::Forms::DockStyle::Fill;
            button1->Text = L""; // Empty text initially
            button1->Tag = gcnew System::Drawing::Point(2, 0); // Centered in the middle of the first row
            button1->BackColor = System::Drawing::Color::White;
            button1->FlatStyle = System::Windows::Forms::FlatStyle::Flat;
            button1->FlatAppearance->BorderSize = 0;
            button1->Font = gcnew System::Drawing::Font(L"Arial", 24.0F, System::Drawing::FontStyle::Bold); // Set font for X and O
            button1->Click += gcnew System::EventHandler(this, &ManagedPyramidTicTacToe::OnCellClick);
            tableLayoutPanel->Controls->Add(button1, 2, 0);

            // Row 2: 3 cells
            tableLayoutPanel->RowStyles->Add(gcnew System::Windows::Forms::RowStyle(System::Windows::Forms::SizeType::Percent, 33.33F));
            for (int i = 1; i < 4; ++i) {
                Button^ button = gcnew Button();
                button->Dock = System::Windows::Forms::DockStyle::Fill;
                button->Text = L""; // Empty text initially
                button->Tag = gcnew System::Drawing::Point(i, 1);
                button->BackColor = System::Drawing::Color::White;
                button->FlatStyle = System::Windows::Forms::FlatStyle::Flat;
                button->FlatAppearance->BorderSize = 0;
                button->Font = gcnew System::Drawing::Font(L"Arial", 24.0F, System::Drawing::FontStyle::Bold);
                button->Click += gcnew System::EventHandler(this, &ManagedPyramidTicTacToe::OnCellClick);
                tableLayoutPanel->Controls->Add(button, i, 1); // Add buttons to the second row
            }

            // Row 3: 5 cells
            tableLayoutPanel->RowStyles->Add(gcnew System::Windows::Forms::RowStyle(System::Windows::Forms::SizeType::Percent, 33.33F));
            for (int i = 0; i < 5; ++i) {
                Button^ button = gcnew Button();
                button->Dock = System::Windows::Forms::DockStyle::Fill;
                button->Text = L""; // Empty text initially
                button->Tag = gcnew System::Drawing::Point(i, 2);
                button->BackColor = System::Drawing::Color::White;
                button->FlatStyle = System::Windows::Forms::FlatStyle::Flat;
                button->FlatAppearance->BorderSize = 0;
                button->Font = gcnew System::Drawing::Font(L"Arial", 24.0F, System::Drawing::FontStyle::Bold);
                button->Click += gcnew System::EventHandler(this, &ManagedPyramidTicTacToe::OnCellClick);
                tableLayoutPanel->Controls->Add(button, i, 2); // Add buttons to the third row
            }

            this->Controls->Add(tableLayoutPanel); // Add the grid below the title
        }

    protected:
        ~ManagedPyramidTicTacToe()
        {
            if (components)
            {
                delete components;
            }
            delete board;
            delete computerPlayer;
        }

    private:
        System::ComponentModel::Container^ components;

#pragma region Windows Form Designer generated code
        void InitializeComponent(void)
        {
            this->components = (gcnew System::ComponentModel::Container());
            this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
            this->ClientSize = System::Drawing::Size(800, 450);
            this->Text = L"Pyramid Tic-Tac-Toe";
        }
#pragma endregion

    private: System::Void OnCellClick(System::Object^ sender, System::EventArgs^ e) {
        Button^ clickedButton = dynamic_cast<Button^>(sender);
        if (clickedButton == nullptr) return;

        // Get the cell coordinates
        System::Drawing::Point cell = safe_cast<System::Drawing::Point>(clickedButton->Tag);
        int x = cell.X, y = cell.Y;

        // Determine the current symbol to place ("X" or "O")
        char currentSymbol = isXturn ? 'X' : 'O';

        // Attempt to update the board
        if (board->update_board(x, y, currentSymbol)) {
            clickedButton->Text = gcnew String(&currentSymbol, 0, 1);
            clickedButton->Enabled = false; // Disable the button once clicked

            // Check for a win first
            if (board->is_win()) {
                MessageBox::Show(isXturn ? L"Player X wins!" : L"Player O wins!", L"Game Over");
                resetGame(); // Reset game if there's a win
            }
            // Then check for a draw
            else if (board->is_draw()) {
                MessageBox::Show(L"It's a draw!", L"Game Over");
                resetGame(); // Reset game if it's a draw
            }
            // If no win or draw, switch turns
            else {
                isXturn = !isXturn;
                if (isHumanVsComputer && !isXturn) {
                    makeAIMove();
                }
            }
        }
        else {
            // If the move was invalid (e.g., cell already filled)
            MessageBox::Show(L"Invalid move. Try again!", L"Error");
        }
    }

    private: void makeAIMove() {
        int x, y;
        computerPlayer->getmove(x, y);
        while (!board->update_board(x, y, 'O')) {
            computerPlayer->getmove(x, y);
        }

        // Update the UI
        for each (Control ^ control in tableLayoutPanel->Controls) {
            Button^ button = dynamic_cast<Button^>(control);
            if (button != nullptr) {
                System::Drawing::Point cell = safe_cast<System::Drawing::Point>(button->Tag);
                if (cell.X == x && cell.Y == y) {
                    button->Text = L"O";
                    button->Enabled = false;
                    break;
                }
            }
        }

        // Check for a win first
        if (board->is_win()) {
            MessageBox::Show(L"Player O wins!", L"Game Over");
            resetGame(); // Reset game if there's a win
        }
        // Then check for a draw
        else if (board->is_draw()) {
            MessageBox::Show(L"It's a draw!", L"Game Over");
            resetGame(); // Reset game if it's a draw
        }
        // If no win or draw, switch turns
        else {
            isXturn = !isXturn;
        }
    }

    private: void resetGame() {
        delete board;
        board = new PyramidBoard<char>();
        for each (Control ^ control in tableLayoutPanel->Controls) {
            Button^ button = dynamic_cast<Button^>(control);
            if (button != nullptr) {
                button->Text = ""; // Clear all button text
                button->Enabled = true; // Enable the button again
            }
        }
        isXturn = true; // Reset to Player X's turn
    }

    };
}