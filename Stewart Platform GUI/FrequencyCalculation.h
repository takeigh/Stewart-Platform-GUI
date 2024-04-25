#pragma once
#include <vector>
#include <string>

#define _USE_MATH_DEFINES
#include <math.h>

class FrequencyCalculation
{
private:
	double frequency;
	double amplitude;
	double duration;
	const double angle = 78 * M_PI / 180;

	std::vector<std::string> positionData;

public:
	std::vector<std::string> calculate(double f, double a, double d);

	void sendData(std::vector<std::string> data);
};

