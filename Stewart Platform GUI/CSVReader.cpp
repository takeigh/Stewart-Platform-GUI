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
	return dataList.size();
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
	ActuatorLengths actuator;

	// Get current row
	std::vector<std::string>& currentLine = this->dataList[row];

	// Take data from current row
	actuator.X = std::stod(currentLine[0]);
	actuator.Y = std::stod(currentLine[1]);
	actuator.Z = std::stod(currentLine[2]);
	actuator.U = std::stod(currentLine[3]);
	actuator.V = std::stod(currentLine[4]);
	actuator.W = std::stod(currentLine[5]);

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