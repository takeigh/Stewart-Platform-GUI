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
	double stepInterval;
	int steps;
	if (frequency <= 5) {
		steps = 32;
	}
	else if (frequency <= 20) {
		steps = 16;
	}
	else {
		steps = 8;
	}

	stepInterval = period / steps;

	std::cout << stepInterval << std::endl;

	std::vector<std::string> periodGroup;

	PlatformWrapper^ platform = gcnew PlatformWrapper();
	double centralLength = platform->GetLengths(200.0);

	// Center point for vertical oscillations
	std::string centerString = std::to_string(centralLength);
	std::string initialization = "G0 X" + centerString + " Y" + centerString + " Z" + centerString + " U" + centerString + " V" + centerString + " W" + centerString + " F20\n";
	std::cout << initialization << std::endl;

	positionData.push_back(initialization);

	// Reset to 5 rather than 0 so drivers don't error out
	std::string reset = "G0 X5 Y5 Z5 U5 V5 W5 F20\n";

	// Calculate positions and velocity for each step
	double prevLength = centralLength;
	for (int i = 0; i < steps; i++) {
		double position = amplitude * sin(frequency * 2 * M_PI * i * stepInterval) + 200;
		
		// Calculate actuator lengths with EulerAngles
		double actuatorLength = platform->GetLengths(position);
		std::string length = std::to_string(actuatorLength);

		// Calculate velocity
		double velocity = (actuatorLength - prevLength) / stepInterval;
		// Only positive velocity
		velocity = abs(velocity);
		std::string velocityString = std::to_string(velocity);

		// Make GCode
		std::string line = "G0 X" + length + " Y" + length + " Z" + length + " U" + length + " V" + length + " W" + length + " F" + velocityString;

		// Add GCode to the periodGroup
		periodGroup.push_back(line);
	}

	// Add the required number of periods to the data
	for (int i = 0; i < (duration * frequency); i++) {
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