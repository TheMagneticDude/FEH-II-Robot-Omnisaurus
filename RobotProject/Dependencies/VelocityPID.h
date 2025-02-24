#pragma once


using namespace std;


class VelocityPID{
  //PID Code structure taken from:https://github.com/dcesiel/Velocity-PID-Example/blob/master/pid.h

	double error;             //Proportional (P)
	double previousError;
	double errorSum;          //Integral     (I)
	double errorRateOfChange; //Derivative   (D)

  float output;

	public:
	int pidCalc(double currentRPM);
	//EFFECTS: Returns motor speed based on PID calculation using current
	//         motor RPM
	
	VelocityPID(double currentRPM);
	//CONSTRUCTOR
	//EFFECTS:  Initializes PID instance
  VelocityPID(double desiredRPM, double currentRPM);
  int pidCalc(double desiredRPM, double currentRPM);
};
