#pragma once

#include "MyForm.h" // Include your main game form

namespace gamesgui {

	using namespace System;
	using namespace System::ComponentModel;
	using namespace System::Collections;
	using namespace System::Windows::Forms;
	using namespace System::Data;
	using namespace System::Drawing;

	public ref class ModeSelection : public System::Windows::Forms::Form
	{
	public:
		ModeSelection(void)
		{
			InitializeComponent();
		}

	protected:
		~ModeSelection()
		{
			if (components)
			{
				delete components;
			}
		}

	private:
		System::Windows::Forms::ComboBox^ player1ModeComboBox;
		System::Windows::Forms::ComboBox^ player2ModeComboBox;
		System::Windows::Forms::Button^ confirmButton;
		System::Windows::Forms::Label^ titleLabel;
		System::Windows::Forms::Label^ player1Label;
		System::Windows::Forms::Label^ player2Label;

		System::ComponentModel::Container^ components;

#pragma region Windows Form Designer generated code
		void InitializeComponent(void)
		{
			this->player1ModeComboBox = gcnew System::Windows::Forms::ComboBox();
			this->player2ModeComboBox = gcnew System::Windows::Forms::ComboBox();
			this->confirmButton = gcnew System::Windows::Forms::Button();
			this->titleLabel = gcnew System::Windows::Forms::Label();
			this->player1Label = gcnew System::Windows::Forms::Label();
			this->player2Label = gcnew System::Windows::Forms::Label();

			// 
			// titleLabel
			// 
			this->titleLabel->Text = L"Select Player Modes";
			this->titleLabel->Font = gcnew System::Drawing::Font(L"Arial", 16, System::Drawing::FontStyle::Bold);
			this->titleLabel->Location = System::Drawing::Point(50, 20);
			this->titleLabel->Size = System::Drawing::Size(200, 30);

			// 
			// player1Label
			// 
			this->player1Label->Text = L"Player 1 Mode:";
			this->player1Label->Location = System::Drawing::Point(50, 60);
			this->player1Label->Size = System::Drawing::Size(100, 20);

			// 
			// player1ModeComboBox
			// 
			this->player1ModeComboBox->DropDownStyle = System::Windows::Forms::ComboBoxStyle::DropDownList;
			this->player1ModeComboBox->Items->AddRange(gcnew cli::array<System::Object^> { L"Human", L"Random Computer", L"Smart Computer (AI)" });
			this->player1ModeComboBox->Location = System::Drawing::Point(50, 80);
			this->player1ModeComboBox->Name = L"player1ModeComboBox";
			this->player1ModeComboBox->Size = System::Drawing::Size(200, 21);

			// 
			// player2Label
			// 
			this->player2Label->Text = L"Player 2 Mode:";
			this->player2Label->Location = System::Drawing::Point(50, 120);
			this->player2Label->Size = System::Drawing::Size(100, 20);

			// 
			// player2ModeComboBox
			// 
			this->player2ModeComboBox->DropDownStyle = System::Windows::Forms::ComboBoxStyle::DropDownList;
			this->player2ModeComboBox->Items->AddRange(gcnew cli::array<System::Object^> { L"Human", L"Random Computer", L"Smart Computer (AI)" });
			this->player2ModeComboBox->Location = System::Drawing::Point(50, 140);
			this->player2ModeComboBox->Name = L"player2ModeComboBox";
			this->player2ModeComboBox->Size = System::Drawing::Size(200, 21);

			// 
			// confirmButton
			// 
			this->confirmButton->Location = System::Drawing::Point(100, 180);
			this->confirmButton->Name = L"confirmButton";
			this->confirmButton->Size = System::Drawing::Size(100, 30);
			this->confirmButton->Text = L"Confirm";
			this->confirmButton->UseVisualStyleBackColor = true;
			this->confirmButton->Click += gcnew System::EventHandler(this, &ModeSelection::OnConfirmButtonClick);

			// 
			// ModeSelection
			// 
			this->AutoScaleDimensions = System::Drawing::SizeF(6, 13);
			this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
			this->ClientSize = System::Drawing::Size(300, 250);
			this->BackColor = System::Drawing::Color::FromArgb(0x2f, 0x4f, 0x4f); // Set background color
			this->Controls->Add(this->titleLabel);
			this->Controls->Add(this->player1Label);
			this->Controls->Add(this->player1ModeComboBox);
			this->Controls->Add(this->player2Label);
			this->Controls->Add(this->player2ModeComboBox);
			this->Controls->Add(this->confirmButton);
			this->Name = L"ModeSelection";
			this->Text = L"Select Player Modes";
		}
#pragma endregion

	private:
		// Event handler for the Confirm button click
		void OnConfirmButtonClick(System::Object^ sender, System::EventArgs^ e)
		{
			String^ player1Mode = player1ModeComboBox->SelectedItem->ToString();
			String^ player2Mode = player2ModeComboBox->SelectedItem->ToString();

			// Navigate to MyForm and pass the player modes
			MyForm^ gameForm = gcnew MyForm(player1Mode, player2Mode);
			this->Hide(); // Hide the ModeSelection form
			gameForm->ShowDialog();
			this->Show(); // Show the ModeSelection form again after MyForm is closed
		}
	};
}
