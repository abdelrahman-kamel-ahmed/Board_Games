#pragma once
#include "BoardGame_Classes.h"
#include "MinMaxPlayer.h"
#include <limits>
#include <algorithm>
#include <iostream>
#include <cstdlib> 
#include<cmath>
#include <vector>
#include <string>
#include <msclr/marshal_cppstd.h>
#using <Microsoft.VisualBasic.dll>

// Game Logic: SUSGameBoard
class SUSGameBoard : public Board<char> {
private:
    std::vector<std::vector<char>> board;
    int player1_score, player2_score;
    int max_moves;

public:
    SUSGameBoard() : player1_score(0), player2_score(0), max_moves(9) {
        rows = 3;
        columns = 3;
        n_moves = 0;
        board.resize(rows, std::vector<char>(columns, ' '));
    }

    bool update_board(int x, int y, char symbol) override {
        if (x < 0 || x >= columns || y < 0 || y >= rows || board[y][x] != ' ') {
            return false; // Invalid move
        }
        board[y][x] = symbol;
        n_moves++;
        return true;
    }

    void display_board() override {
        for (const auto& row : board) {
            for (const auto& cell : row) {
                std::cout << (cell == ' ' ? '.' : cell) << " ";
            }
            std::cout << std::endl;
        }
    }
    int count_sus_sequences(int x, int y, char symbol) {
        int count = 0;

        // Check the row of the latest move
        if (board[y][0] == 'S' && board[y][1] == 'U' && board[y][2] == 'S') count++;

        // Check the column of the latest move
        if (board[0][x] == 'S' && board[1][x] == 'U' && board[2][x] == 'S') count++;

        // Check the main diagonal if the latest move is on it
        if (x == y && board[0][0] == 'S' && board[1][1] == 'U' && board[2][2] == 'S') count++;

        // Check the anti-diagonal if the latest move is on it
        if (x + y == 2 && board[0][2] == 'S' && board[1][1] == 'U' && board[2][0] == 'S') count++;

        return count;
    }

    bool is_win() override {
        return 0;
    }

    bool game_is_over() override {
        return (n_moves >= max_moves);
    }

    bool is_draw() override {
        return (n_moves == max_moves && player1_score == player2_score);
    }

    int get_player1_score() const { return player1_score; }
    int get_player2_score() const { return player2_score; }

    void update_player_score(char symbol, int sus_count, bool isPlayer1Turn) {
        if (isPlayer1Turn) {
            player1_score += sus_count;
        }
        else {
            player2_score += sus_count;
        }
    }


    void determine_winner() {
        if (player1_score > player2_score) {
            std::cout << "Player 1 wins with " << player1_score << " SUS sequences!\n";
        }
        else if (player2_score > player1_score) {
            std::cout << "Player 2 wins with " << player2_score << " SUS sequences!\n";
        }
        else {
            std::cout << "It's a draw! Both players completed " << player1_score << " SUS sequences!\n";
        }
    }
    char get_cell_value(int row, int col) const {
        if (row >= 0 && row < rows && col >= 0 && col < columns) {
            return board[row][col];
        }
        return '\0'; // Invalid cell
    }

    int get_rows() const { return rows; }
    int get_columns() const { return columns; }



};
template <typename T>
std::pair<int, int> getmove(RandomPlayer<T>* player, SUSGameBoard& board) {
    std::vector<std::pair<int, int>> available_moves;

    // Find all empty cells
    for (int row = 0; row < board.get_rows(); ++row) {
        for (int col = 0; col < board.get_columns(); ++col) {
            if (board.get_cell_value(row, col) == ' ') {
                available_moves.emplace_back(row, col);
            }
        }
    }

    if (available_moves.empty()) return { -1, -1 }; // No valid moves

    // Select a random move
    std::pair<int, int> move = available_moves[rand() % available_moves.size()];

    // Randomly select 'S' or 'U' for this turn
    char symbol = (rand() % 2 == 0) ? 'S' : 'U';

    // Update the board with the random symbol
    board.update_board(move.second, move.first, symbol);

    return move;
}



// GUI: SUSGameGUI
public ref class SUSGameGUI : public System::Windows::Forms::Form {
private:
    SUSGameBoard* gameBoard;
    System::Windows::Forms::TableLayoutPanel^ tableLayoutPanel;
    System::Windows::Forms::Label^ statusLabel;
    System::Windows::Forms::Label^ player1ScoreLabel;
    System::Windows::Forms::Label^ player2ScoreLabel;

    Player<int>* player1;
    Player<int>* player2;
    bool isPlayer1Turn = true; // For taking turns

public:
    SUSGameGUI(Player<int>* p1, Player<int>* p2) {
        InitializeComponent();
        InitializeGame(p1, p2);
    }

    ~SUSGameGUI() {
        if (gameBoard) {
            delete gameBoard;
            delete player1;
            delete player2;
        }
    }

private:
    void InitializeGame(Player<int>* p1, Player<int>* p2) {
        gameBoard = new SUSGameBoard();
        player1 = p1;
        player2 = p2;
        isPlayer1Turn = true;
        this->BackColor = System::Drawing::Color::FromArgb(0x2f, 0x4f, 0x4f);

        // Trigger random player if Player 1 is random
        if (dynamic_cast<RandomPlayer<int>*>(player1)) {
            MakeRandomMove(dynamic_cast<RandomPlayer<int>*>(player1));
        }
    }


    void InitializeComponent() {
        this->Text = L"SUS Game";
        this->Size = System::Drawing::Size(400, 500);

        // Player 1 score label
        player1ScoreLabel = gcnew System::Windows::Forms::Label();
        player1ScoreLabel->Dock = System::Windows::Forms::DockStyle::Top;
        player1ScoreLabel->Font = gcnew System::Drawing::Font(L"Arial", 14, System::Drawing::FontStyle::Bold);
        player1ScoreLabel->TextAlign = System::Drawing::ContentAlignment::MiddleLeft;
        player1ScoreLabel->Text = L"Player 1 Score: 0";
        player1ScoreLabel->Height = 30;

        // Player 2 score label
        player2ScoreLabel = gcnew System::Windows::Forms::Label();
        player2ScoreLabel->Dock = System::Windows::Forms::DockStyle::Top;
        player2ScoreLabel->Font = gcnew System::Drawing::Font(L"Arial", 14, System::Drawing::FontStyle::Bold);
        player2ScoreLabel->TextAlign = System::Drawing::ContentAlignment::MiddleLeft;
        player2ScoreLabel->Text = L"Player 2 Score: 0";
        player2ScoreLabel->Height = 30;

        // Status label
        statusLabel = gcnew System::Windows::Forms::Label();
        statusLabel->Dock = System::Windows::Forms::DockStyle::Top;
        statusLabel->Font = gcnew System::Drawing::Font(L"Arial", 16, System::Drawing::FontStyle::Bold);
        statusLabel->TextAlign = System::Drawing::ContentAlignment::MiddleCenter;
        statusLabel->Text = L"Player 1's Turn";
        statusLabel->Height = 50;

        // TableLayoutPanel for game grid
        tableLayoutPanel = gcnew System::Windows::Forms::TableLayoutPanel();
        tableLayoutPanel->RowCount = 3;
        tableLayoutPanel->ColumnCount = 3;
        tableLayoutPanel->Dock = System::Windows::Forms::DockStyle::Fill;
        tableLayoutPanel->CellBorderStyle = System::Windows::Forms::TableLayoutPanelCellBorderStyle::Single;

        for (int i = 0; i < 3; ++i) {
            tableLayoutPanel->RowStyles->Add(gcnew System::Windows::Forms::RowStyle(System::Windows::Forms::SizeType::Percent, 33.33F));
            tableLayoutPanel->ColumnStyles->Add(gcnew System::Windows::Forms::ColumnStyle(System::Windows::Forms::SizeType::Percent, 33.33F));
        }

        for (int i = 0; i < 3; ++i) {
            for (int j = 0; j < 3; ++j) {
                System::Windows::Forms::Button^ button = gcnew System::Windows::Forms::Button();
                button->Dock = System::Windows::Forms::DockStyle::Fill;
                button->Font = gcnew System::Drawing::Font(L"Arial", 20, System::Drawing::FontStyle::Bold);
                button->Tag = gcnew System::Drawing::Point(i, j);
                button->Click += gcnew System::EventHandler(this, &SUSGameGUI::OnCellClick);
                tableLayoutPanel->Controls->Add(button, j, i);
            }
        }

        // Add the controls to the form
        this->Controls->Add(tableLayoutPanel);
        this->Controls->Add(statusLabel);
        this->Controls->Add(player2ScoreLabel);
        this->Controls->Add(player1ScoreLabel);
    }

    void OnCellClick(System::Object^ sender, System::EventArgs^ e) {
        System::Windows::Forms::Button^ clickedButton = safe_cast<System::Windows::Forms::Button^>(sender);
        System::Drawing::Point cell = safe_cast<System::Drawing::Point>(clickedButton->Tag);

        int x = cell.Y;
        int y = cell.X;

        char currentPlayerSymbol;

        // If the current player is a random player, automatically assign 'S' or 'U'
        if (dynamic_cast<RandomPlayer<int>*>(isPlayer1Turn ? player1 : player2)) {
            currentPlayerSymbol = (rand() % 2 == 0) ? 'S' : 'U'; // Randomly assign 'S' or 'U'
        }
        else {
            // Otherwise, ask the human player to input the symbol
            System::String^ input = Microsoft::VisualBasic::Interaction::InputBox(L"Enter your symbol (S or U):", L"Input Symbol", L"S");
            if (input->Length != 1 || (input->ToUpper()[0] != 'S' && input->ToUpper()[0] != 'U')) {
                System::Windows::Forms::MessageBox::Show(L"Invalid input! Please enter 'S' or 'U'.", L"Error");
                return;
            }
            currentPlayerSymbol = static_cast<char>(input->ToUpper()[0]);
        }

        // Perform the move if valid
        if (gameBoard->update_board(x, y, currentPlayerSymbol)) {
            clickedButton->Text = gcnew System::String(currentPlayerSymbol, 1); // Correct

            clickedButton->BackColor = isPlayer1Turn ? System::Drawing::Color::Green : System::Drawing::Color::Orange;
            clickedButton->Enabled = false;

            // Update scores
            int sus_count = gameBoard->count_sus_sequences(x, y, currentPlayerSymbol);
            gameBoard->update_player_score(currentPlayerSymbol, sus_count, isPlayer1Turn);

            UpdateScoreDisplay();

            // Check for game over
            if (gameBoard->game_is_over()) {
                System::String^ result;
                if (gameBoard->get_player1_score() > gameBoard->get_player2_score()) {
                    result = L"Player 1 Wins!";
                }
                else if (gameBoard->get_player1_score() < gameBoard->get_player2_score()) {
                    result = L"Player 2 Wins!";
                }
                else {
                    result = L"It's a Draw!";
                }
                System::Windows::Forms::MessageBox::Show(result, L"Game Over");
                gameBoard->determine_winner();
                ResetGame();
            }
            else {
                // Switch turns
                isPlayer1Turn = !isPlayer1Turn;
                UpdateScoreDisplay();

                // If it's the random player's turn, make a move
                if (!isPlayer1Turn && dynamic_cast<RandomPlayer<int>*>(player2)) {
                    MakeRandomMove(dynamic_cast<RandomPlayer<int>*>(player2));
                }
                else if (isPlayer1Turn && dynamic_cast<RandomPlayer<int>*>(player1)) {
                    MakeRandomMove(dynamic_cast<RandomPlayer<int>*>(player1));
                }
            }
        }
        else {
            System::Windows::Forms::MessageBox::Show(L"Invalid move! The cell is already occupied.", L"Error");
        }
    }


    void MakeRandomMove(RandomPlayer<int>* player) {
        auto move = getmove(player, *gameBoard);

        if (move.first != -1 && move.second != -1) {
            char randomSymbol = gameBoard->get_cell_value(move.first, move.second); // Fetch the actual symbol placed
            UpdateUIAfterMove(move.second, move.first, randomSymbol);

            if (gameBoard->game_is_over()) {
                System::String^ result = isPlayer1Turn ? L"Player 2 wins!" : L"Player 1 wins!";
                System::Windows::Forms::MessageBox::Show(result, L"Game Over");
                ResetGame();
            }
            else {
                isPlayer1Turn = !isPlayer1Turn;
                statusLabel->Text = isPlayer1Turn ? L"Player 1's Turn" : L"Player 2's Turn";

                // Trigger next move if the next player is also a random player
                if (isPlayer1Turn && dynamic_cast<RandomPlayer<int>*>(player1)) {
                    MakeRandomMove(dynamic_cast<RandomPlayer<int>*>(player1));
                }
                else if (!isPlayer1Turn && dynamic_cast<RandomPlayer<int>*>(player2)) {
                    MakeRandomMove(dynamic_cast<RandomPlayer<int>*>(player2));
                }
            }
        }
    }




    void UpdateUIAfterMove(int x, int y, char currentPlayerSymbol) {
        // Access the button corresponding to the cell


        System::Drawing::Color player1Color = System::Drawing::Color::Green;  // Color for Player 1
        System::Drawing::Color player2Color = System::Drawing::Color::Orange; // Color for Player 2



        System::Windows::Forms::Button^ button =
            safe_cast<System::Windows::Forms::Button^>(tableLayoutPanel->GetControlFromPosition(x, y));

        // Update the button's text and appearance
        button->Text = gcnew System::String(currentPlayerSymbol, 1);

        // Assign consistent colors based on the player
        button->BackColor = isPlayer1Turn ? player1Color : player2Color;

        // Disable the button to prevent further moves
        button->Enabled = false;
    }



    void UpdateScoreDisplay() {
        // Update score labels
        player1ScoreLabel->Text = L"Player 1 Score: " + gameBoard->get_player1_score().ToString();
        player2ScoreLabel->Text = L"Player 2 Score: " + gameBoard->get_player2_score().ToString();

        // Update turn status
        statusLabel->Text = (isPlayer1Turn ? L"Player 1's Turn" : L"Player 2's Turn");
    }


    void ResetGame() {
        // Delete the old game board
        delete gameBoard;

        // Recreate a new game board
        gameBoard = new SUSGameBoard();

        // Reset UI buttons
        for each (System::Windows::Forms::Control ^ control in tableLayoutPanel->Controls) {
            System::Windows::Forms::Button^ button = dynamic_cast<System::Windows::Forms::Button^>(control);
            if (button) {
                button->Text = L"";                      // Clear the text
                button->Enabled = true;                 // Enable the button
                button->BackColor = System::Drawing::Color::White; // Reset color
            }
        }

        // Reset the status label
        statusLabel->Text = L"Player 1's Turn";

        // Reset player turn
        isPlayer1Turn = true;

        // If Player 1 is a random player, trigger their move
        if (dynamic_cast<RandomPlayer<int>*>(player1)) {
            MakeRandomMove(dynamic_cast<RandomPlayer<int>*>(player1));
        }
    }

};