#include "ModeSelection.h"
using namespace System;
using namespace System::Windows::Forms;

[STAThreadAttribute]
int main(cli::array<String^>^ args) {
    Application::EnableVisualStyles();
    Application::SetCompatibleTextRenderingDefault(false);

    // Instantiate and run ModeSelection form
    gamesgui::ModeSelection modeSelectionForm;
    Application::Run(% modeSelectionForm);
}