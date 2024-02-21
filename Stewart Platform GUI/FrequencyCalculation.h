#pragma once
#include <vector>
#include <string>

class FrequencyCalculation
{
private:
	double frequency;
	double amplitude;
	double duration;
	std::string axis;

	std::vector<std::vector<std::string>> positionData;

public:
	std::vector<std::vector<std::string>> calculate(double f, double a, double d, std::string axis);

	void printData(std::vector<std::vector<std::string>> data);
};

