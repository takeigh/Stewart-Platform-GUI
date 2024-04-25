#pragma once
#include <vector>

#include "StewartPlatform.h"
#include "Vector3D.h"
#include "EulerAngles.h"
#include "ActuatorLengths.h"
#include "Rotation.h"
#include "CSVReader.h"
#define _USE_MATH_DEFINES
#include <math.h>
#include <windows.h>

class Platform
{
public:
	const double basePlateRadius = 546.1;//base center point to center of actuator mounting bracket
	const double baseMountingAngle = 10.613;//arc angle between actuator mounting brackets from center of platform
	const double platformPlateRadius = 636.1;//platform center point to center of actuator mounting bracket 546.1
	const double platformMountingAngle = 10.613;//arc angle between actuator mounting brackets from center of platform
	const double baseHeight = 850.9;//platform height from ground when at 0,0,0 -> 850.9
	const double maximumLength = 460.0;//Maximum actuator stroke actual is 475. 460 is for safety
	double baseActuatorLength;
	int32_t desiredMoveTime;

	static double xPos;
	static double yPos;
	static double zPos;
	static double uPos;
	static double vPos;
	static double wPos;

	ActuatorLengths aL;
	Vector3D B1, B2, B3, B4, B5, B6;
	Vector3D P1, P2, P3, P4, P5, P6;
	Vector3D L1, L2, L3, L4, L5, L6;

	Platform();

	bool SetPosition(double x, double y, double z, double u, double v, double w);

	bool FollowPath(std::string filename, std::string delimeter);

	void SetMoveTimeMs(int32_t ms);

	ActuatorLengths GetPosition();

	void Move();

	ActuatorLengths CalculateIK(Vector3D XYZ);

	ActuatorLengths CalculateIK(Vector3D XYZ, EulerAngles YPR);
};

