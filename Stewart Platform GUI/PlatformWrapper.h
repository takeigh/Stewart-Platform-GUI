#pragma once

#include <string>

class Platform;
ref class PlatformWrapper
{
private:
	Platform* platform;
public:
	PlatformWrapper();
	virtual ~PlatformWrapper();
	void RunFile(std::string filename, std::string delimeter);
	double GetLengths(double height);
};

