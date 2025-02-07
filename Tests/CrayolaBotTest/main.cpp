#include <FEHLCD.h>
#include <FEHLCD.h>
#include <FEHIO.h>
#include <FEHUtility.h>
#include <FEHMotor.h>
#include <FEHServo.h>
#include <FEHAccel.h>
#include <FEHBattery.h>
#include <FEHBuzzer.h>
#include <FEHRCS.h>
#include <FEHSD.h>
#include <string.h>
#include <stdio.h>

int main(void)
{

    DigitalInputPin LimitFrontLeft(FEHIO::P3_0);
    DigitalInputPin LimitFrontRight(FEHIO::P3_1);
    DigitalInputPin LimitBackLeft(FEHIO::P3_4);
    DigitalInputPin LimitBackRight(FEHIO::P3_5);

    FEHMotor right_motor(FEHMotor::Motor0,9.0);
    FEHMotor left_motor(FEHMotor::Motor1, 9.0);
    while(1){
    LCD.Clear(BLACK);
    LCD.WriteLine("WEEE");

    float speed = 40.0;

    if(!LimitFrontLeft.Value()){
        right_motor.SetPercent(speed);
        LCD.WriteLine("FRONT LEFT");
    }else{
        right_motor.SetPercent(0);
    }

    if(!LimitFrontRight.Value()){
        left_motor.SetPercent(speed);
        LCD.WriteLine("FRONT RIGHT");
    } else {
        left_motor.SetPercent(0);
    }

    if(!LimitBackLeft.Value()){
        right_motor.SetPercent(-speed);
        LCD.WriteLine("BACK LEFT");
    } else {
        right_motor.SetPercent(0);
    }

    if(!LimitBackRight.Value()){
        left_motor.SetPercent(-speed);
        LCD.WriteLine("BACK RIGHT");
    }else{
        left_motor.SetPercent(0);
    }
    
    }
	return 0;
}
