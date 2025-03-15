#include "BoardGame_Classes.h"
#include "MinMaxPlayer.h"
#include <cstdlib> // For rand()
#include <vector>
#include <iostream>

class FiveByFiveTicTacToeBoard : public Board<char> {
private:
    std::vector<std::vector<char>> my5x5board;
    int player1_score, player2_score;
    int max_moves;

public:
    FiveByFiveTicTacToeBoard() : player1_score(0), player2_score(0), max_moves(24) {
        this->rows = 5;
        this->columns = 5;
        this->n_moves = 0;
        my5x5board.resize(rows, std::vector<char>(columns, ' '));
    }

    bool update_board(int x, int y, char symbol) override {
        if (x < 0 || x >= columns || y < 0 || y >= rows || my5x5board[y][x] != ' ') {
            return false; 
        }
        my5x5board[y][x] = symbol; 
        n_moves++;
        return true;
    }

    void display_board() override {
        for (const auto& row : my5x5board) {
            for (const auto& cell : row) {
                std::cout << (cell == ' ' ? '.' : cell) << " ";
            }
            std::cout << std::endl;
        }
    }

    int find_three(char symbol) {
        int count = 0;

        // Check rows
        for (int i = 0; i < rows; ++i) {
            for (int j = 0; j <= columns - 3; ++j) {
                if (my5x5board[i][j] == symbol && my5x5board[i][j + 1] == symbol && my5x5board[i][j + 2] == symbol) {
                    count++;
                }
            }
        }

        // Check columns
        for (int i = 0; i <= rows - 3; ++i) {
            for (int j = 0; j < columns; ++j) {
                if (my5x5board[i][j] == symbol && my5x5board[i + 1][j] == symbol && my5x5board[i + 2][j] == symbol) {
                    count++;
                }
            }
        }

        // Check diagonals 
        for (int i = 0; i <= rows - 3; ++i) {
            for (int j = 0; j <= columns - 3; ++j) {
                if (my5x5board[i][j] == symbol && my5x5board[i + 1][j + 1] == symbol && my5x5board[i + 2][j + 2] == symbol) {
                    count++;
                }
            }
        }

        // Check diagonals
        for (int i = 2; i < rows; ++i) {
            for (int j = 0; j <= columns - 3; ++j) {
                if (my5x5board[i][j] == symbol && my5x5board[i - 1][j + 1] == symbol && my5x5board[i - 2][j + 2] == symbol) {
                    count++;
                }
            }
        }

        return count;
    }

    bool is_win() override {
        player1_score = find_three('X');
        player2_score = find_three('O');
        return (player1_score > player2_score || player2_score > player1_score);
    }

    bool game_is_over() override {
        return n_moves == max_moves;
    }

    bool is_draw() override {
        return n_moves == max_moves && player1_score == player2_score;
    }

    int get_player1_score() const { return player1_score; }
    int get_player2_score() const { return player2_score; }
    char get_cell_value(int row, int col) const {
        if (row >= 0 && row < rows && col >= 0 && col < columns) {
            return my5x5board[row][col];
        }
        return '\0'; 
    }

    int get_rows() const {
        return rows;
    }
    int get_columns() const { 
        return columns; 
    }
};

// Define get_move for RandomPlayer
template <typename T>
std::pair<int, int> getmove(RandomPlayer<T>* player, FiveByFiveTicTacToeBoard& board) {
    std::vector<std::pair<int, int>> available_moves;
    for (int row = 0; row < board.get_rows(); ++row) {
        for (int col = 0; col < board.get_columns(); ++col) {
            if (board.get_cell_value(row, col) == ' ') {
                available_moves.emplace_back(row, col);
            }
        }
    }
    if (available_moves.empty()){

        return { -1, -1 };
    }

    std::pair<int, int> move = available_moves[rand() % available_moves.size()];
    board.update_board(move.second, move.first, player->getsymbol());
    return move;
}

public ref class FiveByFiveTicTacToeGUI : public System::Windows::Forms::Form {
private:
    FiveByFiveTicTacToeBoard* gameBoard;
    Player<int>* player1;
    Player<int>* player2;
    bool isPlayer1Turn;
    System::Windows::Forms::TableLayoutPanel^ tableLayoutPanel;
    System::Windows::Forms::Label^ statusLabel;

public:
    FiveByFiveTicTacToeGUI(Player<int>* p1, Player<int>* p2) {
        InitializeComponent();
        InitializeGame(p1, p2);
    }

    ~FiveByFiveTicTacToeGUI() {
        if (gameBoard) {
            delete gameBoard;
        }
    }

private:
    void InitializeGame(Player<int>* p1, Player<int>* p2) {
        gameBoard = new FiveByFiveTicTacToeBoard();
        player1 = p1;
        player2 = p2;
        isPlayer1Turn = true;
        this->BackColor = System::Drawing::Color::FromArgb(0x2f, 0x4f, 0x4f);
        if (dynamic_cast<RandomPlayer<int>*>(player1)) {
            MakeRandomMove(dynamic_cast<RandomPlayer<int>*>(player1));

        }
    }

    void InitializeComponent() {
        this->Text = L"5x5 Tic Tac Toe";
        this->Size = System::Drawing::Size(600, 700);

        statusLabel = gcnew System::Windows::Forms::Label();
        statusLabel->Dock = System::Windows::Forms::DockStyle::Top;
        statusLabel->Font = gcnew System::Drawing::Font(L"Arial", 16, System::Drawing::FontStyle::Bold);
        statusLabel->TextAlign = System::Drawing::ContentAlignment::MiddleCenter;
        statusLabel->Text = L"Player 1's Turn (X)";
        statusLabel->Height = 50;

        tableLayoutPanel = gcnew System::Windows::Forms::TableLayoutPanel();
        tableLayoutPanel->RowCount = 5;
        tableLayoutPanel->ColumnCount = 5;
        tableLayoutPanel->Dock = System::Windows::Forms::DockStyle::Fill;
        tableLayoutPanel->CellBorderStyle = System::Windows::Forms::TableLayoutPanelCellBorderStyle::Single;

        for (int i = 0; i < 5; ++i) {
            tableLayoutPanel->RowStyles->Add(gcnew System::Windows::Forms::RowStyle(System::Windows::Forms::SizeType::Percent, 20.0F));
            tableLayoutPanel->ColumnStyles->Add(gcnew System::Windows::Forms::ColumnStyle(System::Windows::Forms::SizeType::Percent, 20.0F));
        }

        for (int i = 0; i < 5; ++i) {
            for (int j = 0; j < 5; ++j) {
                System::Windows::Forms::Button^ button = gcnew System::Windows::Forms::Button();
                button->Dock = System::Windows::Forms::DockStyle::Fill;
                button->Font = gcnew System::Drawing::Font(L"Arial", 20, System::Drawing::FontStyle::Bold);
                button->Tag = gcnew System::Drawing::Point(i, j);
                button->Click += gcnew System::EventHandler(this, &FiveByFiveTicTacToeGUI::OnCellClick);
                tableLayoutPanel->Controls->Add(button, j, i);
            }
        }

        this->Controls->Add(tableLayoutPanel);
        this->Controls->Add(statusLabel);
    }

    void OnCellClick(System::Object^ sender, System::EventArgs^ e) {
        System::Windows::Forms::Button^ clickedButton = safe_cast<System::Windows::Forms::Button^>(sender);
        System::Drawing::Point cell = safe_cast<System::Drawing::Point>(clickedButton->Tag);

        int x = cell.Y;
        int y = cell.X;

        char currentPlayerSymbol = isPlayer1Turn ? 'X' : 'O';

        if (gameBoard->update_board(x, y, currentPlayerSymbol)) {
            UpdateUIAfterMove(x, y, currentPlayerSymbol);

            if (gameBoard->game_is_over()) {
                int player1Score = gameBoard->find_three('X');
                int player2Score = gameBoard->find_three('O');
                System::String^ winner;

                if (player1Score > player2Score) {
                    winner = L"Player 1 wins with " + player1Score + L" three-in-a-rows!";
                }
                else if (player2Score > player1Score) {
                    winner = L"Player 2 wins with " + player2Score + L" three-in-a-rows!";
                }
                else {
                    winner = L"It's a draw!";
                }

                System::Windows::Forms::MessageBox::Show(winner, L"Game Over");
                ResetGame();
            }
            else {
                isPlayer1Turn = !isPlayer1Turn;
                statusLabel->Text = isPlayer1Turn ? L"Player 1's Turn (X)" : L"Player 2's Turn (O)";

                if (isPlayer1Turn && dynamic_cast<RandomPlayer<int>*>(player1)) {
                    MakeRandomMove(dynamic_cast<RandomPlayer<int>*>(player1));
                }
                else if (!isPlayer1Turn && dynamic_cast<RandomPlayer<int>*>(player2)) {
                    MakeRandomMove(dynamic_cast<RandomPlayer<int>*>(player2));
                }
            }
        }
        else {
            System::Windows::Forms::MessageBox::Show(L"Invalid move!", L"Error");
        }
    }

    void UpdateUIAfterMove(int x, int y, char currentPlayerSymbol) {
        System::Windows::Forms::Button^ button = safe_cast<System::Windows::Forms::Button^>(tableLayoutPanel->GetControlFromPosition(x, y));
        button->Text = gcnew System::String(currentPlayerSymbol, 1);

        // Color the square based on the player's symbol
        if (currentPlayerSymbol == 'X') {
            button->BackColor = System::Drawing::Color::Red;
        }
        else {
            button->BackColor = System::Drawing::Color::Blue;
        }

        button->Enabled = false;
    }

    void MakeRandomMove(RandomPlayer<int>* player) {
        
        auto move = getmove(player, *gameBoard);
        if (move.first != -1 && move.second != -1) {
            // Update the UI after the move
            UpdateUIAfterMove(move.second, move.first, player->getsymbol());

            // Check if the game is over after the random player's move
            if (gameBoard->game_is_over()) {
                int player1Score = gameBoard->find_three('X');
                int player2Score = gameBoard->find_three('O');
                System::String^ winner;

                if (player1Score > player2Score) {
                    winner = L"Player 1 wins with " + player1Score + L" three-in-a-rows!";
                }
                else if (player2Score > player1Score) {
                    winner = L"Player 2 wins with " + player2Score + L" three-in-a-rows!";
                }
                else {
                    winner = L"It's a draw!";
                }

                System::Windows::Forms::MessageBox::Show(winner, L"Game Over");
                ResetGame();
            }
            else {
                
                isPlayer1Turn = !isPlayer1Turn;
                statusLabel->Text = isPlayer1Turn ? L"Player 1's Turn (X)" : L"Player 2's Turn (O)";
            }
        }
    }


    void ResetGame() {
        if (gameBoard) {
            delete gameBoard;
            gameBoard = nullptr;
        }

        
        gameBoard = new FiveByFiveTicTacToeBoard();

        // Reset UI components
        for each (System::Windows::Forms::Control ^ control in tableLayoutPanel->Controls) {
            System::Windows::Forms::Button^ button = dynamic_cast<System::Windows::Forms::Button^>(control);
            if (button) {
                button->Text = L"";
                button->Enabled = true;
                button->BackColor = System::Drawing::Color::White; 
            }
        }

        isPlayer1Turn = true; // Reset turn
        statusLabel->Text = L"Player 1's Turn (X)";
        if (dynamic_cast<RandomPlayer<int>*>(player1)) {
            MakeRandomMove(dynamic_cast<RandomPlayer<int>*>(player1));
        }
    }
};
