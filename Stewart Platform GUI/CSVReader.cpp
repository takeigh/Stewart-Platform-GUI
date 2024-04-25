#include <fstream>
#include <sstream>
#include "CSVReader.h"
#include "StewartPlatform.h"

CSVReader::CSVReader(std::string fileName, std::string delimeter)
{
	this->fileName = fileName;
	this->delimeter = delimeter;

	this->dataList = Transcribe(this->fileName);
}

int CSVReader::GetDataSize() {
	return dataLines;
}

std::vector<std::vector<std::string>> CSVReader::Transcribe(std::string fileName)
{
	std::vector<std::vector<std::string>> data;

	std::ifstream file(fileName);

	std::string line;

	// Skips the first line
	std::getline(file, line);

	while (std::getline(file, line))
	{
		if (!StewartPlatformGUI::StewartPlatform::running) {
			std::cout << "Cancelled Operation" << std::endl;
			break;
		}

		std::vector<std::string> lineVector = this->SplitLine(line);

		data.push_back(lineVector);

		for each (std::string part in lineVector)
		{
			std::cout << part << " ";
		}
		std::cout << std::endl;
	}

	file.close();
	return data;
}

ActuatorLengths CSVReader::ParseRow(int row) {
	int rowCount = 0;
	int columnCount = 0;
	ActuatorLengths actuator;

	// Get current row
	std::vector<std::string>& currentLine = this->dataList[row];

	// Take data from current row
	for (std::string data : currentLine) {
		if (rowCount == row) {
			if (std::isdigit(data[0]) || data[0] == '-') {
				switch (columnCount) {
				case 0:
					//std::cout << data;
					actuator.X = std::stod(data);
					break;
				case 1:
					actuator.Y = std::stod(data);
					break;
				case 2:
					actuator.Z = std::stod(data);
					break;
				case 3:
					actuator.U = std::stod(data);
					break;
				case 4:
					actuator.V = std::stod(data);
					break;
				case 5:
					actuator.W = std::stod(data);
					break;
				case 6:
					actuator.timeStep = std::stod(data);
					break;
				default:
					break;
				}
			}
		}
	}

	return actuator;
}

std::vector<std::string> CSVReader::SplitLine(std::string& str)
{
	std::vector<std::string> tokens;
	std::stringstream ss(str);
	std::string token;

	while (std::getline(ss, token, ',')) {
		tokens.push_back(token);
	}

	return tokens;
}