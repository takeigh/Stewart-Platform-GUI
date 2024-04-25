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

	double actuatorLength;

	double period = 1.0 / frequency;
	double stepInterval = period / 2;

	std::vector<std::string> periodGroup;

	/*
	std::vector<std::string> zero = {
		"G0 X0 Y0 Z0 U0 V0 W0 F100\n"
	};
	sendData(zero);
	*/

	// Center point for vertical oscillations
	int center = std::round(200.0 / sin(angle));
	std::string centerString = std::to_string(center);
	std::string initialization = "G0 X" + centerString + " Y" + centerString + " Z" + centerString + " U" + centerString + " V" + centerString + " W" + centerString + " F100\n";
	std::cout << initialization << std::endl;
	
	// Reset to 5 rather than 0 so drivers don't error out
	std::string reset = "G0 X5 Y5 Z5 U5 V5 W5 F100\n";

	positionData.push_back(initialization);

	// Find max and min of oscillation (10 * because feedrate is in mm not cm) (+ 200 to oscillate around 20cm)
	double maxPosition = amplitude * sin(frequency * 2 * M_PI * (stepInterval/2)) + 200;
	double minPosition = amplitude * sin(frequency * 2 * M_PI * 3 * (stepInterval/2)) + 200;

	// Acutator is slightly angled
	// Calculate actuator length for desired height
	double theta = sin(angle);
	actuatorLength = maxPosition / theta;
	int wholeNum = std::round(actuatorLength);
	std::string len = std::to_string(wholeNum);

	// Make GCode with max lengths
	std::string maxLine = "G0 X" + len + " Y" + len + " Z" + len + " U" + len + " V" + len + " W" + len;

	actuatorLength = minPosition / theta;
	wholeNum = std::round(actuatorLength);
	len = std::to_string(wholeNum);

	// GCode min lengths
	std::string minLine = "G0 X" + len + " Y" + len + " Z" + len + " U" + len + " V" + len + " W" + len;
	
	// Feed rate is calculated as velocity
	double velocity = (maxPosition - minPosition) / stepInterval;

	// Read everything as positive whole values
	int feedrate = std::abs(std::round(velocity));

	// Attach feedrate to to GCode command with newline character
	maxLine = maxLine + " F" + std::to_string(feedrate) + "\n";
	std::cout << maxLine << std::endl;

	minLine = minLine + " F" + std::to_string(feedrate) + "\n";
	std::cout << minLine << std::endl;

	// Add gcode to periodGroup
	periodGroup.push_back(maxLine);
	periodGroup.push_back(minLine);

	for (int i = 0; i < duration; i++) {
		positionData.insert(positionData.end(), periodGroup.begin(), periodGroup.end());
	}

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
					if (!StewartPlatformGUI::StewartPlatform::running) {
						std::cout << "Cancelled Operation" << std::endl;
						break;
					}

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