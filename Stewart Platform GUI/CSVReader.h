#pragma once
#include <vector>
#include <string>
#include "ActuatorLengths.h"

class CSVReader 
{
public:
	std::string fileName;
	std::string delimeter;

	std::vector<std::vector<std::string>> dataList; //Rows<columns<string data>>

	CSVReader(std::string fileName, std::string delimeter);
	
	int GetDataSize();

	std::vector<std::vector<std::string>> Transcribe(std::string fileName);

	ActuatorLengths ParseRow(int row);

	std::vector<std::string> SplitLine(std::string& str);
};