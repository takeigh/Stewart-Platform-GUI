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

	double period = 1.0 / frequency;
	double stepInterval = period / 2;

	std::vector<std::string> periodGroup;

	PlatformWrapper^ platform = gcnew PlatformWrapper();
	double centralLength = platform->GetLengths(200.0);

	// Center point for vertical oscillations
	std::string centerString = std::to_string(centralLength);
	std::string initialization = "G0 X" + centerString + " Y" + centerString + " Z" + centerString + " U" + centerString + " V" + centerString + " W" + centerString + " F100\n";
	std::cout << initialization << std::endl;

	positionData.push_back(initialization);
	
	// Reset to 5 rather than 0 so drivers don't error out
	std::string reset = "G0 X5 Y5 Z5 U5 V5 W5 F100\n";

	// Find max and min of oscillation (10 * because feedrate is in mm not cm) (+ 200 to oscillate around 20cm)
	double maxPosition = amplitude * sin(frequency * 2 * M_PI * (stepInterval/2)) + 200;
	double minPosition = amplitude * sin(frequency * 2 * M_PI * 3 * (stepInterval/2)) + 200;

	// Acutator is slightly angled
	// Calculate actuator length for desired height
	double maxActuatorLength = platform->GetLengths(maxPosition);
	std::string len = std::to_string(maxActuatorLength);

	// Make GCode with max lengths
	std::string maxLine = "G0 X" + len + " Y" + len + " Z" + len + " U" + len + " V" + len + " W" + len;

	double minActuatorLength = platform->GetLengths(minPosition);
	len = std::to_string(minActuatorLength);

	// GCode min lengths
	std::string minLine = "G0 X" + len + " Y" + len + " Z" + len + " U" + len + " V" + len + " W" + len;
	
	// Feed rate is calculated as velocity
	double velocity = (maxActuatorLength - minActuatorLength) * 2 / stepInterval;

	// Read everything as positive whole values
	double feedrate = std::abs(velocity);

	// Attach feedrate to to GCode command with newline character
	maxLine = maxLine + " F" + std::to_string(feedrate) + "\n";
	std::cout << maxLine << std::endl;

	minLine = minLine + " F" + std::to_string(feedrate) + "\n";
	std::cout << minLine << std::endl;

	// Add gcode to periodGroup
	periodGroup.push_back(maxLine);
	periodGroup.push_back(minLine);

	for (int i = 0; i < (duration * frequency); i++) {
		positionData.insert(positionData.end(), periodGroup.begin(), periodGroup.end());
	}
	
	std::string finalPosition = "G0 X" + centerString + " Y" + centerString + " Z" + centerString + " U" + centerString + " V" + centerString + " W" + centerString + " F" + std::to_string(feedrate) + "\n";
	positionData.push_back(finalPosition);
	
	positionData.push_back(reset);

	return positionData;
}

void FrequencyCalculation::sendData(std::vector<std::string> data) {
	HANDLE serialPort = CreateFile(
		L"COM3",
		GENERIC_READ | GENERIC_WRITE,
		0,
		NULL,
		OPEN_EXISTING,
		FILE_ATTRIBUTE_NORMAL,
		NULL
	);

	if (serialPort == INVALID_HANDLE_VALUE) {
		std::cerr << "Failed to open serial port." << std::endl;
	}
	else
	{
		DCB serialParams = { 0 };
		serialParams.DCBlength = sizeof(serialParams);

		if (!GetCommState(serialPort, &serialParams)) {
			std::cerr << "Error getting serial port state." << std::endl;
		}
		else
		{
			// Configure serial port settings
			serialParams.BaudRate = 115200;  // Baud rate
			serialParams.ByteSize = 8;          // Data bits
			serialParams.StopBits = ONESTOPBIT; // Stop bits
			serialParams.Parity = NOPARITY;     // Parity
			serialParams.fDtrControl = DTR_CONTROL_ENABLE;

			if (!SetCommState(serialPort, &serialParams)) {
				std::cerr << "Error setting serial port state." << std::endl;
			}
			else
			{
				// Loop through commands
				for each (std::string row in data)
				{
					std::cout << "Sending: " << row;

					DWORD bytesWritten;
					if (!WriteFile(serialPort, row.c_str(), row.length(), &bytesWritten, NULL)) {
						std::cerr << "Error writing to serial port." << std::endl;
					}
				}
			}
		}
	}

	CloseHandle(serialPort);
}