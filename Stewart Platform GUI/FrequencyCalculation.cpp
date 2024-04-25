#include "FrequencyCalculation.h"
#include <msclr/marshal_cppstd.h>
#include <windows.h>
#include "StewartPlatform.h"
#include "iostream"

using namespace System;
using namespace System::IO::Ports;
using namespace msclr::interop;

std::vector<std::string> FrequencyCalculation::calculate(double f, double a, double d)
{
	this->frequency = f;
	this->amplitude = a;
	this->duration = d;

	double position;
	double prevPosition = 2000;
	double actuatorLength;
	std::string line;

	double period = 1.0 / frequency;
	double steps = 40;
	double stepInterval = period / steps;

	std::vector<std::string> periodGroup;

	for (int i = 0; i < steps; i++) {
		if (!StewartPlatformGUI::StewartPlatform::running) {
			std::cout << "Cancelled Operation" << std::endl;

			break;
		}

		// Times 10 for millimeters
		position = 10 * amplitude * sin(frequency * 2 * M_PI * i * stepInterval);

		// Make the GCode in the form of "G0 X0 Y0 Z0 U0 V0 W0 F0"

		// Oscillate around 2000 mm (200 cm)
		position += 2000;

		// Acutator is slightly angled
		// Calculate actuator length for desired height
		double theta = sin(angle);
		actuatorLength = position / theta;
		int wholeNum = std::round(actuatorLength);
		std::string len = std::to_string(wholeNum);

		// Make GCode with just actuator lengths
		line = "G0 X" + len + " Y" + len + " Z" + len + " U" + len + " V" + len + " W" + len;

		// Feed rate is calculated as velocity
		double velocity = (position - prevPosition) / stepInterval;

		// Read everything as positive whole values
		int feedrate = std::abs(std::round(velocity));

		// Attach feedrate to to GCode command
		line = line + " F" + std::to_string(feedrate);

		// Add to list of commands
		periodGroup.push_back(line);

		// Set up for next loop
		prevPosition = position;
	}

	for (int i = 0; i < duration; i++) {
		positionData.insert(positionData.end(), periodGroup.begin(), periodGroup.end());
	}

	return positionData;
}

void FrequencyCalculation::sendData(std::vector<std::string> data) {
	SerialPort^ serialPort = gcnew SerialPort("COM3", 115200);
	
	try {
		serialPort->Open();

		for each (std::string row in data)
		{
			if (!StewartPlatformGUI::StewartPlatform::running) {
				std::cout << "Cancelled Operation" << std::endl;
				break;
			}

			// Make a String^ out of std::string for the serial
			System::String^ gcode = gcnew String(msclr::interop::marshal_as<System::String^>(row));

			std::cout << row << std::endl;

			// Serial output
			serialPort->Write(gcode);
		}

	}
	catch (System::UnauthorizedAccessException^ ex)
	{
		std::cout << "Error: Access to the port is denied."")" << std::endl;
	}
	catch (System::IO::IOException^ ex)
	{
		std::cout << "Error: Serial port could not be opened." << std::endl; 
	}

	serialPort->Close();
}