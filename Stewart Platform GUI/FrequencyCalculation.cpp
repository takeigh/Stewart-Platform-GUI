#include "FrequencyCalculation.h"
#include "iostream"
#include <windows.h>

#define _USE_MATH_DEFINES
#include <math.h>
#include "StewartPlatform.h"

std::vector<std::vector<std::string>> FrequencyCalculation::calculate(double f, double a, double d, std::string axis)
{
	frequency = f;
	amplitude = a;
	duration = d;
	axis = axis;

	double position;
	std::vector<std::string> line;

	int partitions = f*200;
	double step = 1.0 / static_cast<double>(partitions);

	std::vector<std::string> line1;
	line1.push_back("X,Y,Z,XR,YR,ZR,Time");
	positionData.push_back(line1);

	for (int i = 0; i * step < d; i++) {
		if (!StewartPlatformGUI::StewartPlatform::running) {
			std::cout << "Cancelled Operation" << std::endl;

			break;
		}

		position = a * sin(f*2* M_PI * step * i);

		line.clear();

		if (axis == "x")
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
		else if (axis == "y")
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
		else if (axis == "z")
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

		line.push_back(std::to_string(step * 1000));

		positionData.push_back(line);
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

		Sleep(time);
	}
}