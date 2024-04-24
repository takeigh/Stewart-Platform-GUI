#include "FrequencyCalculation.h"
#include "iostream"
#include <windows.h>

#define _USE_MATH_DEFINES
#include <math.h>
#include "StewartPlatform.h"

std::vector<std::vector<std::string>> FrequencyCalculation::calculate(double f, double a, double d, std::string axis)
{
	this->frequency = f;
	this->amplitude = a;
	this->duration = d;
	this->axis = axis;

	double position;
	std::vector<std::string> line;

	double period = 1.0 / frequency;
	double steps = 40;
	double stepInterval = period / steps;

	std::vector<std::vector<std::string>> periodGroup;

	std::vector<std::string> line1;
	line1.push_back("X,Y,Z,XR,YR,ZR,Time");
	positionData.push_back(line1);

	for (int i = 0; i < steps; i++) {
		line.clear();

		if (!StewartPlatformGUI::StewartPlatform::running) {
			std::cout << "Cancelled Operation" << std::endl;

			break;
		}

		position = amplitude * sin(frequency * 2 * M_PI * i * stepInterval);

		if (this->axis == "x")
		{
			line.push_back(std::to_string(position));

			for (int j = 0; j < 5; j++)
			{
				if (j == 1)
				{
					line.push_back("200");
				}
				else
				{
					line.push_back("0");
				}
			}

		}
		else if (this->axis == "y")
		{
			line.push_back("0");

			line.push_back(std::to_string(position));

			for (int j = 0; j < 4; j++)
			{
				if (j == 0) {
					line.push_back("200");
				}
				else
				{
					line.push_back("0");
				}
			}
		}
		else if (this->axis == "z")
		{
			for (int j = 0; j < 2; j++)
			{
				line.push_back("0");
			}

			line.push_back(std::to_string(200.0 + position));

			for (int j = 0; j < 3; j++)
			{
				line.push_back("0");
			}
		}

		line.push_back(std::to_string(stepInterval));

		periodGroup.push_back(line);
	}

	for (int i = 0; i < duration; i++) {
		positionData.insert(positionData.end(), periodGroup.begin(), periodGroup.end());
	}

	return positionData;
}

void FrequencyCalculation::printData(std::vector<std::vector<std::string>> data) {
	data.erase(data.begin());
	
	for each (std::vector<std::string> row in data)
	{
		if (!StewartPlatformGUI::StewartPlatform::running) {
			std::cout << "Cancelled Operation" << std::endl;
			break;
		}

		for each (std::string part in row)
		{
			std::cout << part << " ";
		}
		std::cout << std::endl;

		double time = std::stod(row[row.size() - 1]);

		Sleep(time * 1000);
	}
}