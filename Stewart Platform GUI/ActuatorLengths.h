#pragma once

#include "Vector3D.h"
#include <string>

struct ActuatorLengths {
	double U = 0.0;
	double V = 0.0;
	double W = 0.0;
	double X = 0.0;
	double Y = 0.0;
	double Z = 0.0;
	int32_t timeStep = 0;
	bool constraintSuccess = false;

	ActuatorLengths() {}
	ActuatorLengths(double U, double V, double W, double X, double Y, double Z, bool constraintSuccess) : U{ U }, V{ V }, W{ W }, X{ X }, Y{ Y }, Z{ Z }, constraintSuccess{ constraintSuccess } {}
	ActuatorLengths(Vector3D A, Vector3D B, bool constraintSuccess) {
		this->U = A.X;
		this->V = A.Y;
		this->W = A.Z;
		this->X = B.X;
		this->Y = B.Y;
		this->Z = B.Z;
		this->constraintSuccess = constraintSuccess;
	}

	std::string ToString() {
		std::string u = Mathematics::DoubleToCleanString(this->U);
		std::string v = Mathematics::DoubleToCleanString(this->V);
		std::string w = Mathematics::DoubleToCleanString(this->W);
		std::string x = Mathematics::DoubleToCleanString(this->X);
		std::string y = Mathematics::DoubleToCleanString(this->Y);
		std::string z = Mathematics::DoubleToCleanString(this->Z);
		std::string c = constraintSuccess ? "true" : "false";

		return "[" + x + ", " + y + ", " + z + ", " + u + ", " + v + ", " + w + ", " + c + "]";
	}

	std::string GCode() {
		std::string u = std::to_string(U);
		std::string v = std::to_string(V);
		std::string w = std::to_string(W);
		std::string x = std::to_string(X);
		std::string y = std::to_string(Y);
		std::string z = std::to_string(Z);

		return "G0 X" + x + " Y" + y + " Z" + z + " U" + u + " V" + v + " W" + w;
	}
};