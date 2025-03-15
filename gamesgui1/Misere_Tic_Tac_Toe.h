#include "BoardGame_Classes.h"
#include <vector>
#include <iostream>
#include <string>
#include <cstdlib> 

class MisereTicTacToeBoard : public Board<char> {
private:
    std::vector<std::vector<char>> board;
    int player1_score, player2_score;
    int max_moves;

public:
    MisereTicTacToeBoard() : player1_score(0), player2_score(0), max_moves(9) {
        rows = 3;
        columns = 3;
        n_moves = 0;
        board.resize(rows, std::vector<char>(columns, ' '));
    }

    bool update_board(int x, int y, char symbol) override {
        if (x < 0 || x >= columns || y < 0 || y >= rows || board[y][x] != ' ') {
            return false; 
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

    int find_three(char symbol) {
        int count = 0;

        
        for (int i = 0; i < rows; ++i) {
            for (int j = 0; j <= columns - 3; ++j) {
                if (board[i][j] == symbol && board[i][j + 1] == symbol && board[i][j + 2] == symbol) {
                    count++;
                }
            }
        }

       
        for (int i = 0; i <= rows - 3; ++i) {
            for (int j = 0; j < columns; ++j) {
                if (board[i][j] == symbol && board[i + 1][j] == symbol && board[i + 2][j] == symbol) {
                    count++;
                }
            }
        }

        for (int i = 0; i <= rows - 3; ++i) {
            for (int j = 0; j <= columns - 3; ++j) {
                if (board[i][j] == symbol && board[i + 1][j + 1] == symbol && board[i + 2][j + 2] == symbol) {
                    count++;
                }
            }
        }

       
        for (int i = 2; i < rows; ++i) {
            for (int j = 0; j <= columns - 3; ++j) {
                if (board[i][j] == symbol && board[i - 1][j + 1] == symbol && board[i - 2][j + 2] == symbol) {
                    count++;
                }
            }
        }

        return count;
    }

    bool is_win() override {
        player1_score = find_three('X');
        player2_score = find_three('O');

        return player1_score > 0 || player2_score > 0;
    }

    bool game_is_over() override {
        return is_win();
    }

    bool is_draw() override {
        return n_moves == max_moves && player1_score == 0 && player2_score == 0;
    }

    int get_player1_score() const { return player1_score; }
    int get_player2_score() const { return player2_score; }

    char get_cell_value(int row, int col) const {
        if (row >= 0 && row < rows && col >= 0 && col < columns) {
            return board[row][col];
        }
        return '\0';
    }

    int get_rows() const{
        return rows; 
    }
    int get_columns() const {
        return columns; 
    }

    template <typename T>
    friend std::pair<int, int> getmove(RandomPlayer<T>* player, MisereTicTacToeBoard& board) {
        std::vector<std::pair<int, int>> available_moves;
        for (int row = 0; row < board.get_rows(); ++row) {
            for (int col = 0; col < board.get_columns(); ++col) {
                if (board.get_cell_value(row, col) == ' ') {
                    available_moves.emplace_back(row, col);
                }
            }
        }
        if (available_moves.empty()) return { -1, -1 }; // No valid moves

        std::pair<int, int> move = available_moves[rand() % available_moves.size()];
        board.update_board(move.second, move.first, player->getsymbol());
        return move;
    }
};

public ref class MisereTicTacToeGUI : public System::Windows::Forms::Form {
private:
    MisereTicTacToeBoard* MesireBoard;
    Player<int>* player1;
    Player<int>* player2;
    bool isPlayer1Turn;
    System::Windows::Forms::TableLayoutPanel^ tableLayoutPanel;
    System::Windows::Forms::Label^ statusLabel;

public:
    MisereTicTacToeGUI(Player<int>* p1, Player<int>* p2) {
        InitializeComponent();
        InitializeGame(p1, p2);
    }

    ~MisereTicTacToeGUI() {
        if (MesireBoard) {
            delete MesireBoard;
        }
    }

private:
    void InitializeGame(Player<int>* p1, Player<int>* p2) {
        MesireBoard = new MisereTicTacToeBoard();
        player1 = p1;
        player2 = p2;
        isPlayer1Turn = true;
        this->BackColor = System::Drawing::Color::FromArgb(0x2f, 0x4f, 0x4f);
        if (dynamic_cast<RandomPlayer<int>*>(player1)) {
            MakeRandomMove(dynamic_cast<RandomPlayer<int>*>(player1));
        }
    }

    void InitializeComponent() {
        this->Text = L"Misere Tic Tac Toe";
        this->Size = System::Drawing::Size(400, 450);

        statusLabel = gcnew System::Windows::Forms::Label();
        statusLabel->Dock = System::Windows::Forms::DockStyle::Top;
        statusLabel->Font = gcnew System::Drawing::Font(L"Arial", 16, System::Drawing::FontStyle::Bold);
        statusLabel->TextAlign = System::Drawing::ContentAlignment::MiddleCenter;
        statusLabel->Text = L"Player 1's Turn (X)";
        statusLabel->Height = 50;

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
                button->Click += gcnew System::EventHandler(this, &MisereTicTacToeGUI::OnCellClick);
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

        if (MesireBoard->update_board(x, y, currentPlayerSymbol)) {
            UpdateUIAfterMove(x, y, currentPlayerSymbol);

            if (MesireBoard->game_is_over()) {
                System::String^ winner = isPlayer1Turn ? L"Player 2 wins! bec player 1 connected 3 in a row" : L"Player 1 wins! bec player 2 connected 3 in a row";
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
        button->BackColor = currentPlayerSymbol == 'X' ? System::Drawing::Color::Red : System::Drawing::Color::Blue;
        button->Enabled = false;
    }

    void MakeRandomMove(RandomPlayer<int>* player) {
        auto move = getmove(player, *MesireBoard);
        if (move.first != -1 && move.second != -1) {
            UpdateUIAfterMove(move.second, move.first, player->getsymbol());

            if (MesireBoard->game_is_over()) {
                System::String^ winner = isPlayer1Turn ? L"Player 2 wins! bec player 1 connected 3 in a row" : L"Player 1 wins!bec player 2 connected 3 in a row";
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
        if (MesireBoard) {
            delete MesireBoard;
            MesireBoard = nullptr;
        }

        MesireBoard = new MisereTicTacToeBoard();

        for each(System::Windows::Forms::Control ^ control in tableLayoutPanel->Controls) {
            System::Windows::Forms::Button^ button = dynamic_cast<System::Windows::Forms::Button^>(control);
            if (button) {
                button->Text = L"";
                button->Enabled = true;
                button->BackColor = System::Drawing::Color::White;
            }
        }

        isPlayer1Turn = true;
        statusLabel->Text = L"Player 1's Turn (X)";

        if (dynamic_cast<RandomPlayer<int>*>(player1)) {
            MakeRandomMove(dynamic_cast<RandomPlayer<int>*>(player1));
        }
    }
};
