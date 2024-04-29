#include "PlatformWrapper.h"
#include "Platform.h"
#include "ActuatorLengths.h"

PlatformWrapper::PlatformWrapper() {
	platform = new Platform();
}

PlatformWrapper::~PlatformWrapper() {
	delete platform;
}

void PlatformWrapper::RunFile(std::string filename, std::string delimeter) {
	platform->FollowPath(filename, delimeter);
}

double PlatformWrapper::GetLengths(double height) {
	ActuatorLengths a = platform->CalculateIK(Vector3D(0,0,height));

	return a.X;
}