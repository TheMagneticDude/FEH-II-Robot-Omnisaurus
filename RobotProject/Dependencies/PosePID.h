#pragma once


using namespace std;


class PosePID{
  //PID Code structure taken from:https://github.com/dcesiel/Velocity-PID-Example/blob/master/pid.h

	double error;             //Proportional (P)
	double previousError;
	double errorSum;          //Integral     (I)
	double errorRateOfChange; //Derivative   (D)

	float k_P;
	float k_I;
	float k_D; 

  	float output;
	double lastDesiredRPM;

	public:
	int pidCalc(double currentRPM);
	//EFFECTS: Returns target speed needed to reach desired position
	//         motor RPM
	
	PosePID(double currentRPM);
	//CONSTRUCTOR
	//EFFECTS:  Initializes PID instance
  PosePID(double desiredPos, double currentPos);
  float pidCalc(double desiredPos, double currentPos);
  float pidCalcLoopTime(double desiredPos, double currentPos,double loopTime);
  void setPID(float P, float I, float D);
};
