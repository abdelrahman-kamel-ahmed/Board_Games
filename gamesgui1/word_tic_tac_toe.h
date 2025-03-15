#pragma once
#include <iostream>
#include <fstream>
#include <unordered_set>
#include <vector>
#include <msclr/marshal_cppstd.h>
#include <cstdlib>
#include <ctime>
#using <Microsoft.VisualBasic.dll>
#include "BoardGame_Classes.h"
#include <random>

namespace gamesgui {
    using namespace System;
    using namespace System::Windows::Forms;
    using namespace System::Drawing;

    template <typename T>
    class WordTicTacToeBoard : public Board<T> {
    private:
        std::unordered_set<std::string> valid_words;
        std::vector<std::vector<T>> board;

    public:
        int rows, columns, n_moves;

        WordTicTacToeBoard() : rows(3), columns(3), n_moves(0) {
            board.resize(rows, std::vector<T>(columns, ' '));
            load_dictionary("dic.txt");
        }

        void load_dictionary(const std::string& filename) {
            std::ifstream file(filename);
            std::string word;
            while (file >> word) {
                valid_words.insert(word);
            }
        }

        void reset() {
            for (int i = 0; i < rows; ++i) {
                for (int j = 0; j < columns; ++j) {
                    board[i][j] = ' ';
                }
            }
            n_moves = 0;
        }

        bool update_board(int x, int y, T letter) override {
            if (x < 0 || x >= columns || y < 0 || y >= rows || board[y][x] != ' ') return false;
            board[y][x] = letter;
            n_moves++;
            return true;
        }

        bool is_valid_word(const std::string& word) {
            return valid_words.find(word) != valid_words.end();
        }

        bool is_win() override {
            std::vector<std::string> directions = {
                std::string() + board[0][0] + board[0][1] + board[0][2],
                std::string() + board[1][0] + board[1][1] + board[1][2],
                std::string() + board[2][0] + board[2][1] + board[2][2],
                std::string() + board[0][0] + board[1][0] + board[2][0],
                std::string() + board[0][1] + board[1][1] + board[2][1],
                std::string() + board[0][2] + board[1][2] + board[2][2],
                std::string() + board[0][0] + board[1][1] + board[2][2],
                std::string() + board[0][2] + board[1][1] + board[2][0]
            };

            for (const auto& word : directions) {
                if (is_valid_word(word)) return true;
            }
            return false;
        }

        bool is_draw() override {
            return n_moves == rows * columns && !is_win();
        }

        bool game_is_over() override {
            return is_win() || is_draw();
        }

        void display_board() override {
            for (const auto& row : board) {
                for (const auto& cell : row) {
                    std::cout << (cell == ' ' ? '.' : cell) << " ";
                }
                std::cout << std::endl;
            }
        }
        void PlayRandomVsRandom() {
            while (!board->game_is_over()) {
                if (isPlayer1Turn) {
                    MakeRandomMove(dynamic_cast<RandomPlayer<int>*>(player1));
                }
                else {
                    MakeRandomMove(dynamic_cast<RandomPlayer<int>*>(player2));
                }

                // Optional: Add a small delay for UI updates
                System::Threading::Thread::Sleep(500); // 500 ms delay

                Application::DoEvents(); // Refresh UI
            }

            // Display the result once the game is over
            if (board->is_win()) {
                statusLabel->Text = isPlayer1Turn ? "Player 1 Wins!" : "Player 2 Wins!";
                MessageBox::Show(statusLabel->Text + "\nGame Over!", "Game Over");
            }
            else if (board->is_draw()) {
                statusLabel->Text = "It's a Draw!";
                MessageBox::Show("It's a Draw!\nGame Over!", "Game Over");
            }

            reset_game();
        }


        void play_random_move(int& x, int& y, T /*unused*/) {
            std::vector<std::pair<int, int>> empty_cells;

            // Find all empty cells
            for (int i = 0; i < rows; ++i) {
                for (int j = 0; j < columns; ++j) {
                    if (board[i][j] == ' ') {
                        empty_cells.push_back({ i, j });
                    }
                }
            }

            if (!empty_cells.empty()) {
                // Set up random number generators
                static std::random_device rd;  // Seed for randomness
                static std::mt19937 gen(rd()); // Mersenne Twister generator
                std::uniform_int_distribution<> cell_dist(0, empty_cells.size() - 1);
                std::uniform_int_distribution<> letter_dist(0, 25); // Letters A-Z (0-25 range)

                // Choose a random empty cell
                int random_index = cell_dist(gen);
                x = empty_cells[random_index].second; // x-coordinate
                y = empty_cells[random_index].first;  // y-coordinate

                // Generate a random letter A-Z
                char random_letter = 'A' + letter_dist(gen);

                // Update the board with the random letter
                update_board(x, y, random_letter);
            }
        }



        const std::vector<std::vector<T>>& get_board() const {
            return board;
        }
    };


    public ref class WordTicTacToe : public System::Windows::Forms::Form {
    private:
        WordTicTacToeBoard<char>* board;
        System::Windows::Forms::TableLayoutPanel^ tableLayoutPanel;
        System::Windows::Forms::Label^ statusLabel;
        bool isPlayer1Turn;
        Player<int>* player1;
        Player<int>* player2;

    public:
        WordTicTacToe(Player<int>* p1, Player<int>* p2) {
            InitializeComponent();
            board = new WordTicTacToeBoard<char>();
            player1 = p1;
            player2 = p2;
            isPlayer1Turn = true;
            setup_board();

            if (dynamic_cast<RandomPlayer<int>*>(player1)) {
                MakeRandomMove(dynamic_cast<RandomPlayer<int>*>(player1));
            }
            if (dynamic_cast<RandomPlayer<int>*>(player1) && dynamic_cast<RandomPlayer<int>*>(player2)) {
                PlayRandomVsRandom();
            }
        }

    private:
        void InitializeComponent() {
            this->Size = Drawing::Size(500, 550);
            this->Text = "Word Tic-Tac-Toe";
            this->Padding = System::Windows::Forms::Padding(10);
            this->BackColor = Color::LightCyan;

            statusLabel = gcnew System::Windows::Forms::Label();
            statusLabel->Text = "Player 1's Turn";
            statusLabel->Dock = System::Windows::Forms::DockStyle::Top;
            statusLabel->Font = gcnew Drawing::Font("Arial", 10, FontStyle::Bold);
            statusLabel->Height = 30;
            statusLabel->TextAlign = ContentAlignment::MiddleCenter;

            this->Controls->Add(statusLabel);
        }

        void setup_board() {
            tableLayoutPanel = gcnew System::Windows::Forms::TableLayoutPanel();
            tableLayoutPanel->RowCount = board->rows;
            tableLayoutPanel->ColumnCount = board->columns;
            tableLayoutPanel->Dock = System::Windows::Forms::DockStyle::Fill;
            tableLayoutPanel->CellBorderStyle = TableLayoutPanelCellBorderStyle::None;

            for (int i = 0; i < board->rows; ++i) {
                tableLayoutPanel->RowStyles->Add(gcnew RowStyle(SizeType::Percent, 33.3F));
                for (int j = 0; j < board->columns; ++j) {
                    if (i == 0) {
                        tableLayoutPanel->ColumnStyles->Add(gcnew ColumnStyle(SizeType::Percent, 33.3F));
                    }
                    Button^ button = gcnew Button();
                    button->Dock = DockStyle::Fill;
                    button->Font = gcnew Drawing::Font(L"Arial", 24.0F, System::Drawing::FontStyle::Bold);
                    button->BackColor = Color::White;
                    button->Tag = gcnew Point(j, i);
                    button->Click += gcnew EventHandler(this, &WordTicTacToe::OnCellClick);
                    tableLayoutPanel->Controls->Add(button, j, i);
                }
            }

            this->Controls->Add(tableLayoutPanel);
        }

        void OnCellClick(Object^ sender, EventArgs^ e) {
            Button^ button = safe_cast<Button^>(sender);
            Point cell = *dynamic_cast<Point^>(button->Tag);

            // Prompt the user for a letter
            String^ input = Microsoft::VisualBasic::Interaction::InputBox(
                "Enter a letter (A-Z):", "Enter Letter", "", -1, -1);

            if (String::IsNullOrEmpty(input) || input->Length != 1) {
                MessageBox::Show("Invalid input. Please enter a single letter (A-Z).", "Error");
                return;
            }

            // Convert input to uppercase
            char letter = toupper(msclr::interop::marshal_as<std::string>(input)[0]);

            // Check if the move is valid and update the board
            if (board->update_board(cell.X, cell.Y, letter)) {
                button->Text = gcnew String(letter, 1); // Update button text with uppercase letter
                button->Enabled = false;

                if (board->is_win()) {
                    statusLabel->Text = isPlayer1Turn ? "Player 1 Wins!" : "Player 2 Wins!";
                    disable_board();
                    MessageBox::Show(statusLabel->Text + "\nGame Over!", "Game Over");
                    reset_game();
                }
                else if (board->is_draw()) {
                    statusLabel->Text = "It's a Draw!";
                    disable_board();
                    MessageBox::Show("It's a Draw!\nGame Over!", "Game Over");
                    reset_game();
                }
                else {
                    isPlayer1Turn = !isPlayer1Turn;
                    statusLabel->Text = isPlayer1Turn ? "Player 1's Turn" : "Player 2's Turn";

                    if (isPlayer1Turn && dynamic_cast<RandomPlayer<int>*>(player1)) {
                        MakeRandomMove(dynamic_cast<RandomPlayer<int>*>(player1));
                    }
                    else if (!isPlayer1Turn && dynamic_cast<RandomPlayer<int>*>(player2)) {
                        MakeRandomMove(dynamic_cast<RandomPlayer<int>*>(player2));
                    }
                }
            }
            else {
                MessageBox::Show("This cell is already occupied. Try a different move.", "Error");
            }
        }

        void MakeRandomMove(RandomPlayer<int>* player) {
            int x, y;
            board->play_random_move(x, y, player->getsymbol());
            char random_letter = board->get_board()[y][x]; // Fetch the random letter placed by play_random_move
            UpdateUIAfterMove(x, y, random_letter);

            if (board->is_win()) {
                statusLabel->Text = isPlayer1Turn ? "Player 1 Wins!" : "Random Player Wins!";
                disable_board();
                MessageBox::Show(statusLabel->Text + "\nGame Over!", "Game Over");
                reset_game();
            }
            else if (board->is_draw()) {
                statusLabel->Text = "It's a Draw!";
                disable_board();
                MessageBox::Show("It's a Draw!\nGame Over!", "Game Over");
                reset_game();
            }
            else {
                isPlayer1Turn = !isPlayer1Turn;
                statusLabel->Text = isPlayer1Turn ? "Player 1's Turn" : "Random Player's Turn";
            }
        }

        void PlayRandomVsRandom() {
            while (!board->game_is_over()) {
                // Alternate between player 1 and player 2
                if (isPlayer1Turn) {
                    MakeRandomMove(dynamic_cast<RandomPlayer<int>*>(player1));
                }
                else {
                    MakeRandomMove(dynamic_cast<RandomPlayer<int>*>(player2));
                }

                // Optional: Add a small delay for visual feedback
                System::Threading::Thread::Sleep(500);
                Application::DoEvents(); // Refresh the UI
            }

            // Display the game outcome
            if (board->is_win()) {
                statusLabel->Text = isPlayer1Turn ? "Player 1 Wins!" : "Player 2 Wins!";
                MessageBox::Show(statusLabel->Text + "\nGame Over!", "Game Over");
            }
            else if (board->is_draw()) {
                statusLabel->Text = "It's a Draw!";
                MessageBox::Show("It's a Draw!\nGame Over!", "Game Over");
            }

            reset_game();
        }

        void UpdateUIAfterMove(int x, int y, char symbol) {
            Button^ button = safe_cast<Button^>(tableLayoutPanel->GetControlFromPosition(x, y));
            button->Text = gcnew String(symbol, 1);
            button->BackColor = System::Drawing::Color::LightGray;
            button->Enabled = false;
        }

        void reset_game() {
            board->reset();

            for each (Control ^ control in tableLayoutPanel->Controls) {
                Button^ button = dynamic_cast<Button^>(control);
                if (button) {
                    button->Text = "";
                    button->Enabled = true;
                }
            }

            isPlayer1Turn = true;
            statusLabel->Text = "Player 1's Turn";

            if (dynamic_cast<RandomPlayer<int>*>(player1)) {
                MakeRandomMove(dynamic_cast<RandomPlayer<int>*>(player1));
            }
        }

        void disable_board() {
            for each (Control ^ control in tableLayoutPanel->Controls) {
                Button^ button = dynamic_cast<Button^>(control);
                if (button) button->Enabled

                    = false;
            }
        }
    };
}