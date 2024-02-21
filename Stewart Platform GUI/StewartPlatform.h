#pragma once
#include <iostream>
#include "CSVReader.h"
#include <windows.h>
#include <msclr/marshal_cppstd.h>
#include "FrequencyCalculation.h"



namespace StewartPlatformGUI 
{

	using namespace System;
	using namespace System::ComponentModel;
	using namespace System::Collections;
	using namespace System::Windows::Forms;
	using namespace System::Data;
	using namespace System::Drawing;
	using namespace msclr::interop;

	/// <summary>
	/// Summary for StewartPlatform
	/// </summary>
	public ref class StewartPlatform : public System::Windows::Forms::Form
	{
	public:
		StewartPlatform(void)
		{
			InitializeComponent();
		}
	private: System::Windows::Forms::Label^ labelFrequency;
	private: System::Windows::Forms::Label^ labelAmplitude;

	private: System::Windows::Forms::TextBox^ textBoxFrequency;
	private: System::Windows::Forms::TextBox^ textBoxAmplitude;
	private: System::Windows::Forms::Label^ labelDuration;
	private: System::Windows::Forms::TextBox^ textBoxDuration;
	private: System::Windows::Forms::CheckedListBox^ checkedListBoxAxis;

	private: System::Windows::Forms::Label^ labelAxis;

	public:

		static bool running = false;

	protected:
		/// <summary>
		/// Clean up any resources being used.
		/// </summary>
		~StewartPlatform()
		{
			if (components)
			{
				delete components;
			}
		}
	private: System::Windows::Forms::MenuStrip^ menuStrip1;
	private: System::Windows::Forms::ToolStripMenuItem^ menuOptionPositionTime;

	protected:

	private: System::Windows::Forms::ToolStripMenuItem^ menuOptionFrequencyAmplitude;

	private: System::Windows::Forms::Button^ buttonSelectFile;
	private: System::Windows::Forms::Button^ buttonClearFile;
	private: System::Windows::Forms::Label^ labelFilePath;

	private:
		
		double freq;
		double amp;
		double dur;
		System::String^ ax;
		System::String^ filePath;
		bool dataFileInput = true;
		System::Threading::Thread^ reading;
		System::Threading::Thread^ calculationThread;

	private: System::Windows::Forms::Button^ buttonStart;
	private: System::Windows::Forms::Button^ buttonStop;


		/// <summary>
		/// Required designer variable.
		/// </summary>
		System::ComponentModel::Container^ components;

#pragma region Windows Form Designer generated code
		/// <summary>
		/// Required method for Designer support - do not modify
		/// the contents of this method with the code editor.
		/// </summary>
		void InitializeComponent(void)
		{
			this->menuStrip1 = (gcnew System::Windows::Forms::MenuStrip());
			this->menuOptionPositionTime = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->menuOptionFrequencyAmplitude = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->buttonSelectFile = (gcnew System::Windows::Forms::Button());
			this->buttonClearFile = (gcnew System::Windows::Forms::Button());
			this->labelFilePath = (gcnew System::Windows::Forms::Label());
			this->buttonStart = (gcnew System::Windows::Forms::Button());
			this->buttonStop = (gcnew System::Windows::Forms::Button());
			this->labelFrequency = (gcnew System::Windows::Forms::Label());
			this->labelAmplitude = (gcnew System::Windows::Forms::Label());
			this->textBoxFrequency = (gcnew System::Windows::Forms::TextBox());
			this->textBoxAmplitude = (gcnew System::Windows::Forms::TextBox());
			this->labelDuration = (gcnew System::Windows::Forms::Label());
			this->textBoxDuration = (gcnew System::Windows::Forms::TextBox());
			this->checkedListBoxAxis = (gcnew System::Windows::Forms::CheckedListBox());
			this->labelAxis = (gcnew System::Windows::Forms::Label());
			this->menuStrip1->SuspendLayout();
			this->SuspendLayout();
			// 
			// menuStrip1
			// 
			this->menuStrip1->Items->AddRange(gcnew cli::array< System::Windows::Forms::ToolStripItem^  >(2) {
				this->menuOptionPositionTime,
					this->menuOptionFrequencyAmplitude
			});
			this->menuStrip1->Location = System::Drawing::Point(0, 0);
			this->menuStrip1->Name = L"menuStrip1";
			this->menuStrip1->Size = System::Drawing::Size(499, 24);
			this->menuStrip1->TabIndex = 0;
			this->menuStrip1->Text = L"menuStripInputOptions";
			// 
			// menuOptionPositionTime
			// 
			this->menuOptionPositionTime->Name = L"menuOptionPositionTime";
			this->menuOptionPositionTime->Size = System::Drawing::Size(141, 20);
			this->menuOptionPositionTime->Text = L"Position-Time Data File";
			this->menuOptionPositionTime->Click += gcnew System::EventHandler(this, &StewartPlatform::menuOptionPositionTime_Click);
			// 
			// menuOptionFrequencyAmplitude
			// 
			this->menuOptionFrequencyAmplitude->Name = L"menuOptionFrequencyAmplitude";
			this->menuOptionFrequencyAmplitude->Size = System::Drawing::Size(166, 20);
			this->menuOptionFrequencyAmplitude->Text = L"Frequency-Amplitude Input";
			this->menuOptionFrequencyAmplitude->Click += gcnew System::EventHandler(this, &StewartPlatform::menuOptionFrequencyAmplitude_Click);
			// 
			// buttonSelectFile
			// 
			this->buttonSelectFile->Location = System::Drawing::Point(30, 58);
			this->buttonSelectFile->Name = L"buttonSelectFile";
			this->buttonSelectFile->Size = System::Drawing::Size(75, 23);
			this->buttonSelectFile->TabIndex = 1;
			this->buttonSelectFile->Text = L"Select File";
			this->buttonSelectFile->UseVisualStyleBackColor = true;
			this->buttonSelectFile->Click += gcnew System::EventHandler(this, &StewartPlatform::buttonSelectFile_Click);
			// 
			// buttonClearFile
			// 
			this->buttonClearFile->Location = System::Drawing::Point(30, 88);
			this->buttonClearFile->Name = L"buttonClearFile";
			this->buttonClearFile->Size = System::Drawing::Size(75, 23);
			this->buttonClearFile->TabIndex = 2;
			this->buttonClearFile->Text = L"Clear File";
			this->buttonClearFile->UseVisualStyleBackColor = true;
			this->buttonClearFile->Click += gcnew System::EventHandler(this, &StewartPlatform::buttonClearFile_Click);
			// 
			// labelFilePath
			// 
			this->labelFilePath->AutoSize = true;
			this->labelFilePath->Location = System::Drawing::Point(111, 63);
			this->labelFilePath->Name = L"labelFilePath";
			this->labelFilePath->Size = System::Drawing::Size(65, 13);
			this->labelFilePath->TabIndex = 3;
			this->labelFilePath->Text = L"Select a File";
			// 
			// buttonStart
			// 
			this->buttonStart->BackColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(128)), static_cast<System::Int32>(static_cast<System::Byte>(255)),
				static_cast<System::Int32>(static_cast<System::Byte>(128)));
			this->buttonStart->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 10, System::Drawing::FontStyle::Bold, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(0)));
			this->buttonStart->Location = System::Drawing::Point(30, 301);
			this->buttonStart->Name = L"buttonStart";
			this->buttonStart->Size = System::Drawing::Size(179, 97);
			this->buttonStart->TabIndex = 4;
			this->buttonStart->Text = L"Start";
			this->buttonStart->UseVisualStyleBackColor = false;
			this->buttonStart->Click += gcnew System::EventHandler(this, &StewartPlatform::buttonStart_Click);
			// 
			// buttonStop
			// 
			this->buttonStop->BackColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(255)), static_cast<System::Int32>(static_cast<System::Byte>(128)),
				static_cast<System::Int32>(static_cast<System::Byte>(128)));
			this->buttonStop->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 10, System::Drawing::FontStyle::Bold, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(0)));
			this->buttonStop->Location = System::Drawing::Point(287, 301);
			this->buttonStop->Name = L"buttonStop";
			this->buttonStop->Size = System::Drawing::Size(179, 97);
			this->buttonStop->TabIndex = 5;
			this->buttonStop->Text = L"Force Stop";
			this->buttonStop->UseVisualStyleBackColor = false;
			this->buttonStop->Click += gcnew System::EventHandler(this, &StewartPlatform::buttonStop_Click);
			// 
			// labelFrequency
			// 
			this->labelFrequency->AutoSize = true;
			this->labelFrequency->Location = System::Drawing::Point(27, 63);
			this->labelFrequency->Name = L"labelFrequency";
			this->labelFrequency->Size = System::Drawing::Size(79, 13);
			this->labelFrequency->TabIndex = 6;
			this->labelFrequency->Text = L"Frequency (Hz)";
			this->labelFrequency->Visible = false;
			// 
			// labelAmplitude
			// 
			this->labelAmplitude->AutoSize = true;
			this->labelAmplitude->Location = System::Drawing::Point(30, 93);
			this->labelAmplitude->Name = L"labelAmplitude";
			this->labelAmplitude->Size = System::Drawing::Size(76, 13);
			this->labelAmplitude->TabIndex = 7;
			this->labelAmplitude->Text = L"Amplitude (cm)";
			this->labelAmplitude->Visible = false;
			// 
			// textBoxFrequency
			// 
			this->textBoxFrequency->ForeColor = System::Drawing::SystemColors::GrayText;
			this->textBoxFrequency->Location = System::Drawing::Point(114, 61);
			this->textBoxFrequency->Name = L"textBoxFrequency";
			this->textBoxFrequency->Size = System::Drawing::Size(352, 20);
			this->textBoxFrequency->TabIndex = 8;
			this->textBoxFrequency->Text = L"0 to 100";
			this->textBoxFrequency->Visible = false;
			this->textBoxFrequency->GotFocus += gcnew System::EventHandler(this, &StewartPlatform::textBoxFrequency_gotFocus);
			this->textBoxFrequency->LostFocus += gcnew System::EventHandler(this, &StewartPlatform::textBoxFrequency_lostFocus);
			// 
			// textBoxAmplitude
			// 
			this->textBoxAmplitude->ForeColor = System::Drawing::SystemColors::GrayText;
			this->textBoxAmplitude->Location = System::Drawing::Point(114, 90);
			this->textBoxAmplitude->Name = L"textBoxAmplitude";
			this->textBoxAmplitude->Size = System::Drawing::Size(352, 20);
			this->textBoxAmplitude->TabIndex = 9;
			this->textBoxAmplitude->Text = L"0 to 10";
			this->textBoxAmplitude->Visible = false;
			this->textBoxAmplitude->GotFocus += gcnew System::EventHandler(this, &StewartPlatform::textBoxAmplitude_gotFocus);
			this->textBoxAmplitude->LostFocus += gcnew System::EventHandler(this, &StewartPlatform::textBoxAmplitude_lostFocus);
			// 
			// labelDuration
			// 
			this->labelDuration->AutoSize = true;
			this->labelDuration->Location = System::Drawing::Point(44, 120);
			this->labelDuration->Name = L"labelDuration";
			this->labelDuration->Size = System::Drawing::Size(61, 13);
			this->labelDuration->TabIndex = 10;
			this->labelDuration->Text = L"Duration (s)";
			this->labelDuration->Visible = false;
			// 
			// textBoxDuration
			// 
			this->textBoxDuration->ForeColor = System::Drawing::SystemColors::GrayText;
			this->textBoxDuration->Location = System::Drawing::Point(114, 117);
			this->textBoxDuration->Name = L"textBoxDuration";
			this->textBoxDuration->Size = System::Drawing::Size(352, 20);
			this->textBoxDuration->TabIndex = 11;
			this->textBoxDuration->Text = L"0 to 180";
			this->textBoxDuration->Visible = false;
			this->textBoxDuration->GotFocus += gcnew System::EventHandler(this, &StewartPlatform::textBoxDuration_gotFocus);
			this->textBoxDuration->LostFocus += gcnew System::EventHandler(this, &StewartPlatform::textBoxDuration_lostFocus);
			// 
			// checkedListBoxAxis
			// 
			this->checkedListBoxAxis->CheckOnClick = true;
			this->checkedListBoxAxis->FormattingEnabled = true;
			this->checkedListBoxAxis->Items->AddRange(gcnew cli::array< System::Object^  >(3) { L"X-Axis", L"Y-Axis", L"Z-Axis" });
			this->checkedListBoxAxis->Location = System::Drawing::Point(114, 153);
			this->checkedListBoxAxis->Name = L"checkedListBoxAxis";
			this->checkedListBoxAxis->RightToLeft = System::Windows::Forms::RightToLeft::No;
			this->checkedListBoxAxis->Size = System::Drawing::Size(84, 49);
			this->checkedListBoxAxis->TabIndex = 12;
			this->checkedListBoxAxis->Visible = false;
			this->checkedListBoxAxis->SelectedIndexChanged += gcnew System::EventHandler(this, &StewartPlatform::checkedListBoxAxis_SelectedIndexChanged);
			// 
			// labelAxis
			// 
			this->labelAxis->AutoSize = true;
			this->labelAxis->Location = System::Drawing::Point(12, 153);
			this->labelAxis->Name = L"labelAxis";
			this->labelAxis->Size = System::Drawing::Size(91, 13);
			this->labelAxis->TabIndex = 13;
			this->labelAxis->Text = L"Axis of Movement";
			this->labelAxis->Visible = false;
			// 
			// StewartPlatform
			// 
			this->AutoScaleDimensions = System::Drawing::SizeF(6, 13);
			this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
			this->BackColor = System::Drawing::SystemColors::Control;
			this->ClientSize = System::Drawing::Size(499, 425);
			this->Controls->Add(this->labelAxis);
			this->Controls->Add(this->checkedListBoxAxis);
			this->Controls->Add(this->textBoxDuration);
			this->Controls->Add(this->labelDuration);
			this->Controls->Add(this->textBoxAmplitude);
			this->Controls->Add(this->textBoxFrequency);
			this->Controls->Add(this->labelAmplitude);
			this->Controls->Add(this->labelFrequency);
			this->Controls->Add(this->buttonStop);
			this->Controls->Add(this->buttonStart);
			this->Controls->Add(this->labelFilePath);
			this->Controls->Add(this->buttonClearFile);
			this->Controls->Add(this->buttonSelectFile);
			this->Controls->Add(this->menuStrip1);
			this->MainMenuStrip = this->menuStrip1;
			this->Name = L"StewartPlatform";
			this->Text = L"StewartPlatform";
			this->menuStrip1->ResumeLayout(false);
			this->menuStrip1->PerformLayout();
			this->ResumeLayout(false);
			this->PerformLayout();

		}
#pragma endregion
		private: System::Void buttonSelectFile_Click(System::Object^ sender, System::EventArgs^ e)
		{
			OPENFILENAME dlg;
			WCHAR szFile[260] = {0};

			// Parameters for the dialog box
			ZeroMemory(&dlg, sizeof(dlg));
			dlg.lStructSize = sizeof(dlg);
			dlg.hwndOwner = NULL;
			dlg.lpstrFile = szFile;
			dlg.lpstrFile[0] = '\0';
			dlg.lpstrFilter = L"CSV Files (*.csv)\0*.csv\0Text Files (*.txt)\0*.txt\0All Files (*.*)\0*.*\0";
			dlg.nMaxFile = sizeof(szFile);
			dlg.nFilterIndex = 1;
			dlg.lpstrFileTitle = NULL;
			dlg.nMaxFileTitle = 0;
			dlg.lpstrInitialDir = L"C:\\";
			dlg.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST | OFN_NOCHANGEDIR;

			if (GetOpenFileName(&dlg) == TRUE) {
				filePath = msclr::interop::marshal_as<String^>(szFile);

				labelFilePath->Text = filePath;
			}
		};
	
		private: System::Void buttonClearFile_Click(System::Object^ sender, System::EventArgs^ e) 
		{
			filePath = nullptr;

			labelFilePath->Text = "Select a File";
		}
	
		private: System::Void buttonStop_Click(System::Object^ sender, System::EventArgs^ e) 
		{
			if (running) {
				running = false;

				MessageBox::Show("Terminated Operation", "Force Stop");
			}
		}


		private: System::Void buttonStart_Click(System::Object^ sender, System::EventArgs^ e) 
		{
			if (dataFileInput) 
			{
				if (filePath != nullptr)
				{
					reading = gcnew System::Threading::Thread(gcnew System::Threading::ThreadStart(this, &StewartPlatform::readFile));
					reading->Start();

					running = true;
				}
				else
				{
					MessageBox::Show("Please Select a File First", "Error: No File Found");
				}
			}
			else
			{
				std::string frequency = marshal_as<std::string>(textBoxFrequency->Text);  
				std::string amplitude = marshal_as<std::string>(textBoxAmplitude->Text); 
				std::string duration = marshal_as<std::string>(textBoxDuration->Text); 

				if (validFrequencyInputs(frequency, amplitude, duration))
				{
					double f = std::stod(frequency);
					double a = std::stod(amplitude);
					double d = std::stod(duration);

					std::string axis = "0";
					std::string temp;
					System::String^ itemText;

					for (int i = 0; i < checkedListBoxAxis->Items->Count; i++) {
						if (checkedListBoxAxis->GetItemChecked(i)) {
							itemText = checkedListBoxAxis->Items[i]->ToString();
							temp = marshal_as<std::string>(itemText);
						}
					}

					if (temp == "X-Axis")
					{
						axis = "x";
					}
					else if (temp == "Y-Axis")
					{
						axis = "y";
					}
					else if (temp == "Z-Axis")
					{
						axis = "z";
					}

					if (axis != "0")
					{
						// THREAD START
						freq = f;
						amp = a;
						dur = d;
						ax = itemText;

						calculationThread = gcnew System::Threading::Thread(gcnew System::Threading::ThreadStart(this, &StewartPlatform::calculationProcess));
						calculationThread->Start();

						running = true;
					}
					else
					{
						MessageBox::Show("No axis selected.", "Error: Invalid Axis");
					}
				}
			}
		}

		private: System::Void readFile() 
		{
			auto temp = this->filePath;
			std::string stringFilePath = marshal_as<std::string>(temp);

			CSVReader reader(stringFilePath, ",");
		}
			
		private: System::Void menuOptionPositionTime_Click(System::Object^ sender, System::EventArgs^ e) 
		{
			dataFileInput = true;

			visibilityChange(dataFileInput);
		}

		private: System::Void menuOptionFrequencyAmplitude_Click(System::Object^ sender, System::EventArgs^ e) 
		{
			dataFileInput = false;

			visibilityChange(dataFileInput);
		}

		private: System::Void visibilityChange(bool state) 
		{
			//Data File Input Components
			buttonSelectFile->Visible = state;
			buttonClearFile->Visible = state;
			labelFilePath->Visible = state;

			//Frequency-Amplitude Input Components
			labelFrequency->Visible = !state;
			textBoxFrequency->Visible = !state;
			labelAmplitude->Visible = !state;
			textBoxAmplitude->Visible = !state;
			labelDuration->Visible = !state;
			textBoxDuration->Visible = !state;
			labelAxis->Visible = !state;
			checkedListBoxAxis->Visible = !state;

		}

		private: bool validFrequencyInputs(std::string frequency, std::string amplitude, std::string duration)
		{
			try
			{
				size_t length;

				double f = std::stod(frequency, &length);

				if (length != frequency.size())
				{
					MessageBox::Show("Please fill in all the fields.", "Error: Empty Field(s)");
					return false;
				}

				double a = std::stod(amplitude, &length);

				if (length != amplitude.size())
				{
					MessageBox::Show("Please fill in all the fields.", "Error: Empty Field(s)");
					return false;
				}

				double d = std::stod(duration, &length);

				if (length != frequency.size())
				{
					MessageBox::Show("Please fill in all the fields.", "Error: Empty Field(s)");
					return false;
				}

				if (f > 0 && f <= 100)
				{
					if (a > 0 && a <= 10) {
						if (d > 0 && d <= 180)
						{
							return true;
						}
						else
						{
							if (d <= 0)
							{
								MessageBox::Show("Duration must be greater than 0.", "Error: Duration too Short");
							}
							else
							{
								MessageBox::Show("Duration must be less than or equal to 180.", "Error: Duration too Long");
							}
						}
					}
					else
					{
						if (a <= 0)
						{
							MessageBox::Show("Amplitude must be greater than 0.", "Error: Amplitude too Small");
						}
						else
						{
							MessageBox::Show("Amplitude must be less than or equal to 10.", "Error: Amplitude too Large");
						}
					}
				}
				else
				{
					if (f <= 0)
					{
						MessageBox::Show("Frequency must be greater than 0.", "Error: Frequency too Small");
					}
					else
					{
						MessageBox::Show("Frequency must be less than or equal to 100.", "Error: Frequency too Large");
					}
				}

			}
			catch (const std::exception& e)
			{
				MessageBox::Show("Please fill in all the fields.", "Error: Empty Field(s)");
				return false;
			}

			return false;
		}
		
		private: System::Void checkedListBoxAxis_SelectedIndexChanged(System::Object^ sender, System::EventArgs^ e) 
		{
			for (int i = 0; i < checkedListBoxAxis->Items->Count; i++)
			{
				if (i != checkedListBoxAxis->SelectedIndex)
				{
					checkedListBoxAxis->SetItemChecked(i, false);
				}
			}
		}

		private: System::Void calculationProcess() 
		{
			FrequencyCalculation calculator;

			auto temp = this->ax;
			std::string temp2 = marshal_as<std::string>(temp);

			std::string axis;

			if (temp2 == "X-Axis")
			{
				axis = "x";
			}
			else if (temp2 == "Y-Axis")
			{
				axis = "y";
			}
			else if (temp2 == "Z-Axis")
			{
				axis = "z";
			}

			std::vector<std::vector<std::string>> data = calculator.calculate(freq, amp, dur, axis);

			if (running)
			{
				calculator.printData(data);
			}
		}

		private: System::Void textBoxFrequency_gotFocus(System::Object^ sender, System::EventArgs^ e) 
		{
			TextBox^ textBox = dynamic_cast<TextBox^>(sender);
			if (textBox->Text == "0 to 100") {
				textBox->Text = "";
				textBox->ForeColor = System::Drawing::SystemColors::WindowText;
			}
		}

		private: System::Void textBoxFrequency_lostFocus(System::Object^ sender, System::EventArgs^ e)
		{
			TextBox^ textBox = dynamic_cast<TextBox^>(sender);
			if (textBox->Text == "") {
				textBox->Text = "0 to 100";
				textBox->ForeColor = System::Drawing::SystemColors::GrayText;
			}
		}

		private: System::Void textBoxAmplitude_gotFocus(System::Object^ sender, System::EventArgs^ e) 
		{
			TextBox^ textBox = dynamic_cast<TextBox^>(sender);
			if (textBox->Text == "0 to 10") {
				textBox->Text = "";
				textBox->ForeColor = System::Drawing::SystemColors::WindowText;
			}
		}

		private: System::Void textBoxAmplitude_lostFocus(System::Object^ sender, System::EventArgs^ e)
		{
			TextBox^ textBox = dynamic_cast<TextBox^>(sender);
			if (textBox->Text == "") {
				textBox->Text = "0 to 10";
				textBox->ForeColor = System::Drawing::SystemColors::GrayText;
			}
		}

		private: System::Void textBoxDuration_gotFocus(System::Object^ sender, System::EventArgs^ e)
		{
			TextBox^ textBox = dynamic_cast<TextBox^>(sender);
			if (textBox->Text == "0 to 180") {
				textBox->Text = "";
				textBox->ForeColor = System::Drawing::SystemColors::WindowText;
			}
		}

		private: System::Void textBoxDuration_lostFocus(System::Object^ sender, System::EventArgs^ e)
		{
			TextBox^ textBox = dynamic_cast<TextBox^>(sender);
			if (textBox->Text == "") {
				textBox->Text = "0 to 180";
				textBox->ForeColor = System::Drawing::SystemColors::GrayText;
			}
		}
	};
}