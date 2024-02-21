#include "StewartPlatform.h"

using namespace System;
using namespace System::Windows::Forms;

int main(array<String^>^ args)
{
	Application::EnableVisualStyles();
	Application::SetCompatibleTextRenderingDefault(false);
	StewartPlatformGUI::StewartPlatform StewartPlatform;
	Application::Run(% StewartPlatform);

	return 0;
}
