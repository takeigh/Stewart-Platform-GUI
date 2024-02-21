#include "CSVReader.h"
#include "iostream"
#include <sstream>
#include <windows.h>
#include "StewartPlatform.h"


CSVReader::CSVReader(std::string fileName, std::string delimeter) 
{
	this->fileName = fileName;
	this->delimeter = delimeter;

	this->dataList = transcribe(this->fileName);
}

std::vector<std::vector<std::string>> CSVReader::transcribe(std::string fileName)
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

		std::vector<std::string> lineVector = this->splitLine(line);
		
		data.push_back(lineVector);

		for each (std::string part in lineVector)
		{
			std::cout << part << " ";
		}

		std::cout << std::endl;

		int time = std::stoi(lineVector[lineVector.size() - 1]);

		Sleep(time);
	}

	file.close();
	return data;
}

std::vector<std::string> CSVReader::splitLine(std::string& str) 
{
	std::vector<std::string> tokens;
	std::stringstream ss(str);
	std::string token;

	while (std::getline(ss, token, ',')) {
		tokens.push_back(token);
	}

	return tokens;
}