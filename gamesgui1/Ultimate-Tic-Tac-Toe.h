#pragma once

#include "UltimateMainBoard.h"
#include <vector>
#include <limits>
#include <cstdlib>
#include "MinMaxPlayer.h"
#include <algorithm>

namespace gamesgui {

    using namespace System;
    using namespace System::ComponentModel;
    using namespace System::Collections;
    using namespace System::Windows::Forms;
    using namespace System::Data;
    using namespace System::Drawing;
    using namespace System::Collections::Generic;

    template <typename T>
    public ref class UltimateTicTacToe : public System::Windows::Forms::Form
    {
    public:
        UltimateTicTacToe(Player<T>* p1, Player<T>* p2)
        {
            InitializeComponent();
            CreateGrid();
            CreateBoldBorders();
            game = new UltimateMainBoard();
            player1 = p1;
            player2 = p2;
            currentPlayer = player1->getsymbol();
            isPlayer1Turn = true;

            // If player 1 is AI, make the first move
            if (dynamic_cast<RandomPlayer<T>*>(player1) || dynamic_cast<X_O_MinMax_Player<T>*>(player1)) {
                startAITimer();
            }
        }

    protected:
        ~UltimateTicTacToe()
        {
            if (components)
            {
                delete components;
            }
            delete game;
            delete player1;
            delete player2;
        }

    private:
        System::ComponentModel::Container^ components;
        cli::array<System::Windows::Forms::Button^, 2>^ buttons;
        UltimateMainBoard* game;
        char currentPlayer;
        Player<T>* player1;
        Player<T>* player2;
        bool isPlayer1Turn;
        System::Windows::Forms::Timer^ aiTimer;

#pragma region Windows Form Designer generated code
        void InitializeComponent(void)
        {
            this->components = gcnew System::ComponentModel::Container();
            this->Size = System::Drawing::Size(500, 500);
            this->Text = L"UltimateTicTacToe";
            this->Padding = System::Windows::Forms::Padding(0);
            this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
        }
#pragma endregion

        void CreateGrid()
        {
            int gridSize = 9;
            int buttonSize = 50; // Each button is 50x50 pixels
            buttons = gcnew cli::array<System::Windows::Forms::Button^, 2>(gridSize, gridSize);

            for (int row = 0; row < gridSize; row++)
            {
                for (int col = 0; col < gridSize; col++)
                {
                    buttons[row, col] = gcnew System::Windows::Forms::Button();

                    // Set button properties
                    buttons[row, col]->Size = System::Drawing::Size(buttonSize, buttonSize);
                    buttons[row, col]->Location = System::Drawing::Point(col * buttonSize, row * buttonSize);
                    buttons[row, col]->Text = "";
                    buttons[row, col]->Tag = row.ToString() + "," + col.ToString();
                    buttons[row, col]->Font = gcnew System::Drawing::Font(L"Arial", 16, FontStyle::Bold);

                    // Add click event handler
                    buttons[row, col]->Click += gcnew System::EventHandler(this, &UltimateTicTacToe::OnButtonClick);

                    // Add the button to the form
                    this->Controls->Add(buttons[row, col]);
                }
            }
        }

        void CreateBoldBorders()
        {
            int gridSize = 9;
            int buttonSize = 50;
            int lineWidth = 4; // Bold line width

            // Create vertical bold lines
            for (int i = 1; i <= 2; i++)
            {
                Panel^ verticalLine = gcnew Panel();
                verticalLine->BackColor = Color::Black;
                verticalLine->Size = System::Drawing::Size(lineWidth, gridSize * buttonSize);
                verticalLine->Location = System::Drawing::Point(i * 3 * buttonSize - lineWidth / 2, 0);

                // Add the line to the form
                this->Controls->Add(verticalLine);
                verticalLine->BringToFront();
            }

            // Create horizontal bold lines
            for (int i = 1; i <= 2; i++)
            {
                Panel^ horizontalLine = gcnew Panel();
                horizontalLine->BackColor = Color::Black;
                horizontalLine->Size = System::Drawing::Size(gridSize * buttonSize, lineWidth);
                horizontalLine->Location = System::Drawing::Point(0, i * 3 * buttonSize - lineWidth / 2);

                // Add the line to the form
                this->Controls->Add(horizontalLine);
                horizontalLine->BringToFront();
            }
        }

        void OnButtonClick(Object^ sender, EventArgs^ e)
        {
            Button^ clickedButton = safe_cast<Button^>(sender);
            String^ tag = clickedButton->Tag->ToString();
            List<String^>^ indices = gcnew List<String^>(tag->Split(','));

            int row = Int32::Parse(indices[0]);
            int col = Int32::Parse(indices[1]);

            // Update game state
            if (game->update_board(row, col, currentPlayer))
            {
                // Update button text based on the current player
                clickedButton->Text = gcnew String(currentPlayer, 1);

                // Check for game over conditions
                if (game->is_win())
                {
                    MessageBox::Show("Game Over! " + gcnew String(currentPlayer, 1) + " wins!");
                    // Optionally, reset the game
                    ResetGame();
                }
                else if (game->is_draw())
                {
                    MessageBox::Show("Game Over! It's a draw!");
                    // Optionally, reset the game
                    ResetGame();
                }
                else
                {
                    // Switch player
                    currentPlayer = (currentPlayer == 'X') ? 'O' : 'X';
                    isPlayer1Turn = !isPlayer1Turn;

                    // If it's the AI's turn, make a move
                    if (!isPlayer1Turn && (dynamic_cast<RandomPlayer<T>*>(player2) || dynamic_cast<X_O_MinMax_Player<T>*>(player2)))
                    {
                        startAITimer();
                    }
                }
            }
        }

        void ResetGame()
        {
            for (int row = 0; row < buttons->GetLength(0); row++)
            {
                for (int col = 0; col < buttons->GetLength(1); col++)
                {
                    buttons[row, col]->Text = "";
                }
            }
            delete game;
            game = new UltimateMainBoard();
            currentPlayer = 'X';
            isPlayer1Turn = true;

            // If player 1 is AI, make the first move
            if (dynamic_cast<RandomPlayer<T>*>(player1) || dynamic_cast<X_O_MinMax_Player<T>*>(player1)) {
                startAITimer();
            }
        }

        void startAITimer() {
            aiTimer = gcnew System::Windows::Forms::Timer();
            aiTimer->Interval = 500;
            aiTimer->Tick += gcnew System::EventHandler(this, &UltimateTicTacToe::onAITimerTick);
            aiTimer->Start();
        }

        void onAITimerTick(System::Object^ sender, System::EventArgs^ e) {
            if (game->game_is_over()) {
                aiTimer->Stop();
                return;
            }
            makeAIMove();
        }

        void makeAIMove() {
            int move = -1; // Initialize move
            Player<T>* currentPlayer = isPlayer1Turn ? player1 : player2;

            if (dynamic_cast<RandomPlayer<T>*>(currentPlayer)) {
                move = get_move<T>(dynamic_cast<RandomPlayer<T>*>(currentPlayer), *game);
            }
            else if (dynamic_cast<X_O_MinMax_Player<T>*>(currentPlayer)) {
                move = get_move<T>(dynamic_cast<X_O_MinMax_Player<T>*>(currentPlayer), *game);
            }

            if (move != -1) {
                int moveRow = move / 9;
                int moveCol = move % 9;
                game->update_board(moveRow, moveCol, currentPlayer->getsymbol());
                buttons[moveRow, moveCol]->Text = gcnew String(currentPlayer->getsymbol(), 1);

                // Check for game over conditions after AI move
                if (game->is_win())
                {
                    MessageBox::Show("Game Over! " + gcnew String(currentPlayer->getsymbol(), 1) + " wins!");
                    ResetGame();
                }
                else if (game->is_draw())
                {
                    MessageBox::Show("Game Over! It's a draw!");
                    ResetGame();
                }
                else
                {
                    isPlayer1Turn = !isPlayer1Turn;
                    currentPlayer = isPlayer1Turn ? player1 : player2;
                    this->currentPlayer = currentPlayer->getsymbol();

                    // If it's the AI's turn, make a move
                    if (!isPlayer1Turn && (dynamic_cast<RandomPlayer<T>*>(player2) || dynamic_cast<X_O_MinMax_Player<T>*>(player2))) {
                        startAITimer();
                    }
                }
            }
        }
    };
}