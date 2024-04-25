#include "Platform.h"
#include "StewartPlatform.h"

double Platform::xPos = 0;
double Platform::yPos = 0;
double Platform::zPos = 0;
double Platform::uPos = 0;
double Platform::vPos = 0;
double Platform::wPos = 0;

Platform::Platform() 
{
	double baseActuatorLength = 0;//leave this alone, used in IK
	int32_t desiredMoveTime = 0x00000000;

	//Calculating base and platform actuator points. 
	B1 = Vector3D(basePlateRadius * cos((baseMountingAngle / 2.0) * M_PI / 180.0), -basePlateRadius * sin((baseMountingAngle / 2.0) * M_PI / 180.0), 0);
	B2 = Vector3D(basePlateRadius * cos((baseMountingAngle / 2.0) * M_PI / 180.0), basePlateRadius * sin((baseMountingAngle / 2.0) * M_PI / 180.0), 0);
	B3 = Vector3D(-basePlateRadius * cos((60.0 + baseMountingAngle / 2.0) * M_PI / 180.0), basePlateRadius * sin((60.0 + baseMountingAngle / 2.0) * M_PI / 180.0), 0);
	B4 = Vector3D(-basePlateRadius * cos((60.0 - baseMountingAngle / 2.0) * M_PI / 180.0), basePlateRadius * sin((60.0 - baseMountingAngle / 2.0) * M_PI / 180.0), 0);
	B5 = Vector3D(-basePlateRadius * cos((60.0 - baseMountingAngle / 2.0) * M_PI / 180.0), -basePlateRadius * sin((60.0 - baseMountingAngle / 2.0) * M_PI / 180.0), 0);
	B6 = Vector3D(-basePlateRadius * cos((60.0 + baseMountingAngle / 2.0) * M_PI / 180.0), -basePlateRadius * sin((60.0 + baseMountingAngle / 2.0) * M_PI / 180.0), 0);

	P1 = Vector3D(platformPlateRadius * cos((60.0 - platformMountingAngle / 2.0) * M_PI / 180.0), -platformPlateRadius * sin((60.0 - platformMountingAngle / 2.0) * M_PI / 180.0), 0);
	P2 = Vector3D(platformPlateRadius * cos((60.0 - platformMountingAngle / 2.0) * M_PI / 180.0), platformPlateRadius * sin((60.0 - platformMountingAngle / 2.0) * M_PI / 180.0), 0);
	P3 = Vector3D(platformPlateRadius * cos((60.0 + platformMountingAngle / 2.0) * M_PI / 180.0), platformPlateRadius * sin((60.0 + platformMountingAngle / 2.0) * M_PI / 180.0), 0);
	P4 = Vector3D(-platformPlateRadius * cos((platformMountingAngle / 2.0) * M_PI / 180.0), platformPlateRadius * sin((platformMountingAngle / 2.0) * M_PI / 180.0), 0);
	P5 = Vector3D(-platformPlateRadius * cos((platformMountingAngle / 2.0) * M_PI / 180.0), -platformPlateRadius * sin((platformMountingAngle / 2.0) * M_PI / 180.0), 0);
	P6 = Vector3D(platformPlateRadius * cos((60.0 + platformMountingAngle / 2.0) * M_PI / 180.0), -platformPlateRadius * sin((60.0 + platformMountingAngle / 2.0) * M_PI / 180.0), 0);

	baseActuatorLength = CalculateIK(Vector3D(0, 0, 0)).U;
}

bool Platform::SetPosition(double x, double y, double z, double u, double v, double w) {
	ActuatorLengths aL = CalculateIK(Vector3D(x, y, z), EulerAngles(Vector3D(u, v, w), EulerConstants::EulerOrderXYZR));//Change to relative rotation
	//std::cout << aL.
	// () << std::endl;

	xPos = x;
	yPos = y;
	zPos = z;
	uPos = u;
	vPos = v;
	wPos = w;

	return aL.constraintSuccess;
}

bool Platform::FollowPath(std::string filename, std::string delimeter) {
	CSVReader fileReader(filename, delimeter);
	int numRows = fileReader.GetDataSize();

	for (int i = 0; i < numRows; i++) {
		if (!StewartPlatformGUI::StewartPlatform::running) {
			std::cout << "Cancelled Operation" << std::endl;
			return false;
		}

		ActuatorLengths nextMove = fileReader.ParseRow(i);
		if (Platform::SetPosition(nextMove.X, nextMove.Y, nextMove.Z, nextMove.U, nextMove.V, nextMove.W)) {
			Platform::SetMoveTimeMs(nextMove.timeStep);
			Platform::Move();
			std::cout << "Moving to waypoint at row " << i + 1 << ": " << GetPosition().ToString() << " in " << nextMove.timeStep << " Ms" << std::endl;
		}
		else {
			std::cout << "[!] Invalid waypoint at row " << i + 1 << ": " << GetPosition().ToString() << std::endl;
		}
		// Shouldn't need a sleep timer
		//Sleep((int32_t)nextMove.timeStep);
	}
	return true;
}

void Platform::SetMoveTimeMs(int32_t ms) {
	desiredMoveTime = ms;
}

ActuatorLengths Platform::GetPosition() {
	ActuatorLengths al;
	//Need to update to actually read from platform position buffer.
	al.X = xPos;
	al.Y = yPos;
	al.Z = zPos;
	al.U = uPos;
	al.V = vPos;
	al.W = wPos;

	return al;
}

void Platform::Move() {
	// TODO
}

ActuatorLengths Platform::CalculateIK(Vector3D XYZ) {
	EulerAngles ypr = EulerAngles(Vector3D(0, 0, 0), EulerConstants::EulerOrderXYZR);
	return CalculateIK(XYZ, ypr);
}


ActuatorLengths Platform::CalculateIK(Vector3D XYZ, EulerAngles YPR) {
	XYZ.Z += baseHeight;

	//std::cout << YPR.ToString() << std::endl;
	//These two lines rotate the input coordinate system to the proper orientation. Both in the translational and rotational axis.
	YPR.Angles = Rotation(EulerAngles(Vector3D(0, 0, 60), EulerConstants::EulerOrderXYZR)).GetQuaternion().RotateVector(YPR.Angles);
	XYZ = Rotation(EulerAngles(Vector3D(30, 0, 0), EulerConstants::EulerOrderXYZR)).GetQuaternion().RotateVector(XYZ);

	EulerAngles noYaw = YPR;
	noYaw.Angles.X = 0;

	Quaternion noYawQ = Rotation(noYaw).GetQuaternion();
	Quaternion q = Rotation(YPR).GetQuaternion();

	XYZ = noYawQ.RotateVector(XYZ);

	L1 = q.RotateVector(P1) + XYZ - B1;
	L2 = q.RotateVector(P2) + XYZ - B2;
	L3 = q.RotateVector(P3) + XYZ - B3;
	L4 = q.RotateVector(P4) + XYZ - B4;
	L5 = q.RotateVector(P5) + XYZ - B5;
	L6 = q.RotateVector(P6) + XYZ - B6;

	// Ordering of L1 - L6 is based off of physical platform actuators
	double w1 = L1.GetLength() - baseActuatorLength;
	double x1 = L2.GetLength() - baseActuatorLength;
	double y1 = L3.GetLength() - baseActuatorLength;
	double z1 = L4.GetLength() - baseActuatorLength;
	double u1 = L5.GetLength() - baseActuatorLength;
	double v1 = L6.GetLength() - baseActuatorLength;

	bool withinConstraints = w1 >= 0 && w1 < maximumLength &&
		x1 >= 0 && x1 < maximumLength &&
		y1 >= 0 && y1 < maximumLength &&
		z1 >= 0 && z1 < maximumLength &&
		u1 >= 0 && u1 < maximumLength &&
		v1 >= 0 && v1 < maximumLength;
	//Ordering of L1 - L6 is based off of physical platform actuators
	return ActuatorLengths(x1, y1, z1, u1, v1, w1, withinConstraints);
}