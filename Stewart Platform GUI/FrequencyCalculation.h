#pragma once
#include <vector>
#include <string>

class FrequencyCalculation
{
private:
	double frequency;
	double amplitude;
	double duration;
	double angle;

	std::vector<std::string> positionData;

public:
	std::vector<std::string> calculate(double f, double a, double d);

	void sendData(std::vector<std::string> data);
};

