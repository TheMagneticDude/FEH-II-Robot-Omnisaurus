#include <FEHLCD.h>
#include <FEHLCD.h>
#include <FEHIO.h>
#include <FEHUtility.h>
#include <FEHMotor.h>
#include <FEHServo.h>
#include <FEHAccel.h>
#include <FEHBattery.h>
#include <FEHBuzzer.h>
#include <FEHSD.h>
#include <string.h>
#include <stdio.h>

int main(void)
{

    DigitalInputPin LimitFrontLeft(FEHIO::P3_0);
    DigitalInputPin LimitFrontRight(FEHIO::P0_0);
    DigitalInputPin LimitBackLeft(FEHIO::P3_1);
    DigitalInputPin LimitBackRight(FEHIO::P0_1);

    FEHMotor right_motor(FEHMotor::Motor1,9.0);
    FEHMotor left_motor(FEHMotor::Motor2, 9.0);

    
	return 0;
}
