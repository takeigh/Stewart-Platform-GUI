#pragma once
#include <fstream>
#include <vector>
#include <string>
#include <algorithm>

class CSVReader {
private:
	std::string fileName;
	std::string delimeter;

	std::vector<std::vector<std::string>> dataList; //Rows<columns<string data>>

	std::vector<std::vector<std::string>> transcribe(std::string fileName);

	std::vector<std::string> splitLine(std::string& str);

public:
	CSVReader(std::string fileName, std::string dellimeter);
};