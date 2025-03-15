#pragma once
#include "Four-In-a-Row.h"
#include "pyramid_tic_tac_toe.h"
#include "Numerical-Tic-Tac-Toe.h"
#include "3x3X_O.h"
#include "MinMaxPlayer.h"
#include "5x5_Tic_Tac_Toe.h" 
#include "word_tic_tac_toe.h"
#include "Misere_Tic_Tac_Toe.h"
#include "SUS_Tic_Tac_Toe.h"
#include"Ultimate-Tic-Tac-Toe.h"
namespace gamesgui {

    using namespace System;
    using namespace System::ComponentModel;
    using namespace System::Collections;
    using namespace System::Windows::Forms;
    using namespace System::Data;
    using namespace System::Drawing;

    public ref class MyForm : public System::Windows::Forms::Form
    {
    public:
        MyForm(String^ player1Mode, String^ player2Mode)
        {
            InitializeComponent();
            InitializePlayers(player1Mode, player2Mode);
            this->player1Mode = player1Mode;
            this->player2Mode = player2Mode;
        }

    protected:
        ~MyForm()
        {
            if (components)
            {
                delete components;
            }
        }

    private:
        System::Windows::Forms::Label^ label1;
        System::Windows::Forms::Button^ pyramic;
        System::Windows::Forms::Button^ fourinr;
        System::Windows::Forms::Button^ fiveXfive;
        System::Windows::Forms::Button^ wordticbtn;
        System::Windows::Forms::Button^ numerical_btn;
        System::Windows::Forms::Button^ misere_btn;
        System::Windows::Forms::Button^ ultimate_btn;
        System::Windows::Forms::Button^ sus_btn;
        System::Windows::Forms::Label^ label2;

        System::ComponentModel::Container^ components;

        Player<int>* player1;
        Player<int>* player2;
        String^ player1Mode;
    private: System::Windows::Forms::FlowLayoutPanel^ flowLayoutPanel1;
    private: System::Windows::Forms::FlowLayoutPanel^ flowLayoutPanel2;
    private: System::Windows::Forms::FlowLayoutPanel^ flowLayoutPanel3;
    private: System::Windows::Forms::FlowLayoutPanel^ flowLayoutPanel4;
           String^ player2Mode;

           void InitializePlayers(String^ player1Mode, String^ player2Mode)
           {
               player1 = CreatePlayer(player1Mode, 'X');
               player2 = CreatePlayer(player2Mode, 'O');
           }

           Player<int>* CreatePlayer(String^ mode, char symbol)
           {
               if (mode == "Human")
               {
                   return new X_O_Player<int>("Player", symbol);
               }
               else if (mode == "Random Computer")
               {
                   return new X_O_Random_Player<int>(symbol);
               }
               else if (mode == "Smart Computer (AI)")
               {
                   return new X_O_MinMax_Player<int>(symbol);
               }
               return nullptr;
           }

#pragma region Windows Form Designer generated code
           void InitializeComponent(void)
           {
               this->pyramic = (gcnew System::Windows::Forms::Button());
               this->label1 = (gcnew System::Windows::Forms::Label());
               this->fourinr = (gcnew System::Windows::Forms::Button());
               this->fiveXfive = (gcnew System::Windows::Forms::Button());
               this->wordticbtn = (gcnew System::Windows::Forms::Button());
               this->numerical_btn = (gcnew System::Windows::Forms::Button());
               this->misere_btn = (gcnew System::Windows::Forms::Button());
               this->ultimate_btn = (gcnew System::Windows::Forms::Button());
               this->sus_btn = (gcnew System::Windows::Forms::Button());
               this->label2 = (gcnew System::Windows::Forms::Label());
               this->flowLayoutPanel1 = (gcnew System::Windows::Forms::FlowLayoutPanel());
               this->flowLayoutPanel2 = (gcnew System::Windows::Forms::FlowLayoutPanel());
               this->flowLayoutPanel3 = (gcnew System::Windows::Forms::FlowLayoutPanel());
               this->flowLayoutPanel4 = (gcnew System::Windows::Forms::FlowLayoutPanel());
               this->SuspendLayout();
               // 
               // pyramic
               // 
               this->pyramic->BackColor = System::Drawing::Color::Transparent;
               this->pyramic->Font = (gcnew System::Drawing::Font(L"Palatino Linotype", 14.25F, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point,
                   static_cast<System::Byte>(0)));
               this->pyramic->Location = System::Drawing::Point(264, 235);
               this->pyramic->Margin = System::Windows::Forms::Padding(5);
               this->pyramic->Name = L"pyramic";
               this->pyramic->Size = System::Drawing::Size(329, 111);
               this->pyramic->TabIndex = 0;
               this->pyramic->Text = L"PYRAMIC TIC-TAC-TOE";
               this->pyramic->UseVisualStyleBackColor = false;
               this->pyramic->Click += gcnew System::EventHandler(this, &MyForm::pyramic_Click);
               // 
               // label1
               // 
               this->label1->AutoSize = true;
               this->label1->BackColor = System::Drawing::Color::White;
               this->label1->BorderStyle = System::Windows::Forms::BorderStyle::Fixed3D;
               this->label1->Font = (gcnew System::Drawing::Font(L"Rockwell", 18, System::Drawing::FontStyle::Bold, System::Drawing::GraphicsUnit::Point,
                   static_cast<System::Byte>(0)));
               this->label1->Location = System::Drawing::Point(479, 132);
               this->label1->Margin = System::Windows::Forms::Padding(5, 0, 5, 0);
               this->label1->Name = L"label1";
               this->label1->Size = System::Drawing::Size(279, 40);
               this->label1->TabIndex = 2;
               this->label1->Text = L"SELECT A GAME ";
               this->label1->Click += gcnew System::EventHandler(this, &MyForm::label1_Click);
               // 
               // fourinr
               // 
               this->fourinr->BackColor = System::Drawing::Color::Transparent;
               this->fourinr->Font = (gcnew System::Drawing::Font(L"Palatino Linotype", 14.25F, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point,
                   static_cast<System::Byte>(0)));
               this->fourinr->Location = System::Drawing::Point(669, 235);
               this->fourinr->Margin = System::Windows::Forms::Padding(5);
               this->fourinr->Name = L"fourinr";
               this->fourinr->Size = System::Drawing::Size(329, 111);
               this->fourinr->TabIndex = 3;
               this->fourinr->Text = L"4-IN-A-ROW";
               this->fourinr->UseVisualStyleBackColor = false;
               this->fourinr->Click += gcnew System::EventHandler(this, &MyForm::fourinr_Click);
               // 
               // fiveXfive
               // 
               this->fiveXfive->BackColor = System::Drawing::Color::Transparent;
               this->fiveXfive->Font = (gcnew System::Drawing::Font(L"Palatino Linotype", 14.25F, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point,
                   static_cast<System::Byte>(0)));
               this->fiveXfive->Location = System::Drawing::Point(267, 580);
               this->fiveXfive->Margin = System::Windows::Forms::Padding(5);
               this->fiveXfive->Name = L"fiveXfive";
               this->fiveXfive->Size = System::Drawing::Size(329, 111);
               this->fiveXfive->TabIndex = 4;
               this->fiveXfive->Text = L"5x5 TIC-TAC-TOE";
               this->fiveXfive->UseVisualStyleBackColor = false;
               this->fiveXfive->Click += gcnew System::EventHandler(this, &MyForm::fiveXfive_Click);
               // 
               // wordticbtn
               // 
               this->wordticbtn->BackColor = System::Drawing::Color::Transparent;
               this->wordticbtn->Font = (gcnew System::Drawing::Font(L"Palatino Linotype", 14.25F, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point,
                   static_cast<System::Byte>(0)));
               this->wordticbtn->Location = System::Drawing::Point(264, 409);
               this->wordticbtn->Margin = System::Windows::Forms::Padding(5);
               this->wordticbtn->Name = L"wordticbtn";
               this->wordticbtn->Size = System::Drawing::Size(329, 111);
               this->wordticbtn->TabIndex = 5;
               this->wordticbtn->Text = L"WORD TIC-TAC-TOE";
               this->wordticbtn->UseVisualStyleBackColor = false;
               this->wordticbtn->Click += gcnew System::EventHandler(this, &MyForm::wordticbtn_Click);
               // 
               // numerical_btn
               // 
               this->numerical_btn->BackColor = System::Drawing::Color::Transparent;
               this->numerical_btn->Font = (gcnew System::Drawing::Font(L"Palatino Linotype", 14.25F, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point,
                   static_cast<System::Byte>(0)));
               this->numerical_btn->Location = System::Drawing::Point(667, 409);
               this->numerical_btn->Margin = System::Windows::Forms::Padding(5);
               this->numerical_btn->Name = L"numerical_btn";
               this->numerical_btn->Size = System::Drawing::Size(329, 111);
               this->numerical_btn->TabIndex = 6;
               this->numerical_btn->Text = L"NUMERICAL TIC-TAC-TOE";
               this->numerical_btn->UseVisualStyleBackColor = false;
               this->numerical_btn->Click += gcnew System::EventHandler(this, &MyForm::numerical_btn_Click);
               // 
               // misere_btn
               // 
               this->misere_btn->BackColor = System::Drawing::Color::Transparent;
               this->misere_btn->Font = (gcnew System::Drawing::Font(L"Palatino Linotype", 14.25F, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point,
                   static_cast<System::Byte>(0)));
               this->misere_btn->Location = System::Drawing::Point(669, 580);
               this->misere_btn->Margin = System::Windows::Forms::Padding(5);
               this->misere_btn->Name = L"misere_btn";
               this->misere_btn->Size = System::Drawing::Size(329, 111);
               this->misere_btn->TabIndex = 7;
               this->misere_btn->Text = L"MISERE TIC-TAC-TOE";
               this->misere_btn->UseVisualStyleBackColor = false;
               this->misere_btn->Click += gcnew System::EventHandler(this, &MyForm::misere_btn_Click);
               // 
               // ultimate_btn
               // 
               this->ultimate_btn->BackColor = System::Drawing::Color::Transparent;
               this->ultimate_btn->Font = (gcnew System::Drawing::Font(L"Palatino Linotype", 14.25F, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point,
                   static_cast<System::Byte>(0)));
               this->ultimate_btn->Location = System::Drawing::Point(264, 754);
               this->ultimate_btn->Margin = System::Windows::Forms::Padding(5);
               this->ultimate_btn->Name = L"ultimate_btn";
               this->ultimate_btn->Size = System::Drawing::Size(329, 111);
               this->ultimate_btn->TabIndex = 8;
               this->ultimate_btn->Text = L"ULTIMATE TIC-TAC-TOE";
               this->ultimate_btn->UseVisualStyleBackColor = false;
               this->ultimate_btn->Click += gcnew System::EventHandler(this, &MyForm::ultimate_btn_Click);
               // 
               // sus_btn
               // 
               this->sus_btn->BackColor = System::Drawing::Color::Transparent;
               this->sus_btn->Font = (gcnew System::Drawing::Font(L"Palatino Linotype", 14.25F, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point,
                   static_cast<System::Byte>(0)));
               this->sus_btn->Location = System::Drawing::Point(669, 754);
               this->sus_btn->Margin = System::Windows::Forms::Padding(5);
               this->sus_btn->Name = L"sus_btn";
               this->sus_btn->Size = System::Drawing::Size(329, 111);
               this->sus_btn->TabIndex = 9;
               this->sus_btn->Text = L"SUS ";
               this->sus_btn->UseVisualStyleBackColor = false;
               this->sus_btn->Click += gcnew System::EventHandler(this, &MyForm::sus_btn_Click);
               // 
               // label2
               // 
               this->label2->AutoSize = true;
               this->label2->BackColor = System::Drawing::Color::Transparent;
               this->label2->BorderStyle = System::Windows::Forms::BorderStyle::Fixed3D;
               this->label2->Font = (gcnew System::Drawing::Font(L"Rockwell", 48, System::Drawing::FontStyle::Bold, System::Drawing::GraphicsUnit::Point,
                   static_cast<System::Byte>(0)));
               this->label2->ForeColor = System::Drawing::Color::OrangeRed;
               this->label2->Location = System::Drawing::Point(324, 11);
               this->label2->Margin = System::Windows::Forms::Padding(5, 0, 5, 0);
               this->label2->Name = L"label2";
               this->label2->Size = System::Drawing::Size(575, 98);
               this->label2->TabIndex = 10;
               this->label2->Text = L"GAME MENU";
               // 
               // flowLayoutPanel1
               // 
               this->flowLayoutPanel1->BackColor = System::Drawing::Color::Gold;
               this->flowLayoutPanel1->Location = System::Drawing::Point(625, 230);
               this->flowLayoutPanel1->Margin = System::Windows::Forms::Padding(4);
               this->flowLayoutPanel1->Name = L"flowLayoutPanel1";
               this->flowLayoutPanel1->Size = System::Drawing::Size(13, 635);
               this->flowLayoutPanel1->TabIndex = 12;
               // 
               // flowLayoutPanel2
               // 
               this->flowLayoutPanel2->BackColor = System::Drawing::Color::OrangeRed;
               this->flowLayoutPanel2->Location = System::Drawing::Point(264, 370);
               this->flowLayoutPanel2->Margin = System::Windows::Forms::Padding(4);
               this->flowLayoutPanel2->Name = L"flowLayoutPanel2";
               this->flowLayoutPanel2->Size = System::Drawing::Size(732, 12);
               this->flowLayoutPanel2->TabIndex = 13;
               // 
               // flowLayoutPanel3
               // 
               this->flowLayoutPanel3->BackColor = System::Drawing::Color::Red;
               this->flowLayoutPanel3->ForeColor = System::Drawing::SystemColors::ButtonFace;
               this->flowLayoutPanel3->Location = System::Drawing::Point(267, 543);
               this->flowLayoutPanel3->Margin = System::Windows::Forms::Padding(4);
               this->flowLayoutPanel3->Name = L"flowLayoutPanel3";
               this->flowLayoutPanel3->Size = System::Drawing::Size(732, 12);
               this->flowLayoutPanel3->TabIndex = 14;
               // 
               // flowLayoutPanel4
               // 
               this->flowLayoutPanel4->BackColor = System::Drawing::Color::Firebrick;
               this->flowLayoutPanel4->Location = System::Drawing::Point(264, 716);
               this->flowLayoutPanel4->Margin = System::Windows::Forms::Padding(4);
               this->flowLayoutPanel4->Name = L"flowLayoutPanel4";
               this->flowLayoutPanel4->Size = System::Drawing::Size(732, 12);
               this->flowLayoutPanel4->TabIndex = 14;
               // 
               // MyForm
               // 
               this->AutoScaleDimensions = System::Drawing::SizeF(8, 16);
               this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
               this->BackColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(47)), static_cast<System::Int32>(static_cast<System::Byte>(79)),
                   static_cast<System::Int32>(static_cast<System::Byte>(79)));
               this->ClientSize = System::Drawing::Size(1312, 937);
               this->Controls->Add(this->flowLayoutPanel4);
               this->Controls->Add(this->flowLayoutPanel3);
               this->Controls->Add(this->flowLayoutPanel2);
               this->Controls->Add(this->flowLayoutPanel1);
               this->Controls->Add(this->label2);
               this->Controls->Add(this->sus_btn);
               this->Controls->Add(this->ultimate_btn);
               this->Controls->Add(this->misere_btn);
               this->Controls->Add(this->numerical_btn);
               this->Controls->Add(this->wordticbtn);
               this->Controls->Add(this->fiveXfive);
               this->Controls->Add(this->fourinr);
               this->Controls->Add(this->label1);
               this->Controls->Add(this->pyramic);
               this->Margin = System::Windows::Forms::Padding(4);
               this->Name = L"MyForm";
               this->Text = L"Game Menu";
               this->ResumeLayout(false);
               this->PerformLayout();

           }
#pragma endregion

    private:
    private:
        System::Void pyramic_Click(System::Object^ sender, System::EventArgs^ e) {
            // Determine if the game is human vs computer
            bool humanVsComputer = (player1Mode == "Human" && player2Mode != "Human") || (player1Mode != "Human" && player2Mode == "Human");

            // Create and show the Pyramid Tic-Tac-Toe game form
            ManagedPyramidTicTacToe^ Pyramic = gcnew ManagedPyramidTicTacToe(humanVsComputer);
            Pyramic->Show();
        }


        System::Void fourinr_Click(System::Object^ sender, System::EventArgs^ e) {
            // Implement the logic to start the 4-In-A-Row game
            FourInaRow^ fourInaRowForm = gcnew FourInaRow(player1, player2);
            fourInaRowForm->Show();
        }

        System::Void numerical_btn_Click(System::Object^ sender, System::EventArgs^ e) {
            // Implement the logic to start the Numerical Tic-Tac-Toe game
            NumericalTicTacToe^ numericalForm = gcnew NumericalTicTacToe();
            numericalForm->Show();
        }

        System::Void fiveXfive_Click(System::Object^ sender, System::EventArgs^ e) {
            // Implement the logic to start the 5x5 Tic-Tac-Toe game
            FiveByFiveTicTacToeGUI^ fiveByFiveForm = gcnew FiveByFiveTicTacToeGUI(player1, player2);
            fiveByFiveForm->Show();
        }

        System::Void label1_Click(System::Object^ sender, System::EventArgs^ e) {
        }
        System::Void wordticbtn_Click(System::Object^ sender, System::EventArgs^ e) {
            // Implement the logic to start the Word Tic-Tac-Toe game
            WordTicTacToe^ wordTicTacToeForm = gcnew WordTicTacToe(player1, player2);
            wordTicTacToeForm->Show();
        }
        System::Void misere_btn_Click(System::Object^ sender, System::EventArgs^ e) {
            // Implement the logic to start the Misere Tic-Tac-Toe game
            MisereTicTacToeGUI^ misereTicTacToeForm = gcnew  MisereTicTacToeGUI(player1, player2);
            misereTicTacToeForm->Show();
        }

    private: System::Void sus_btn_Click(System::Object^ sender, System::EventArgs^ e) {
        SUSGameGUI^ susGameForm = gcnew SUSGameGUI(player1, player2);
        susGameForm->Show();

    }
           System::Void ultimate_btn_Click(System::Object^ sender, System::EventArgs^ e) {
               UltimateTicTacToe<int>^ UltimateTicTacToeForm = gcnew UltimateTicTacToe<int>(player1, player2);
               UltimateTicTacToeForm->Show();
           }
};
}