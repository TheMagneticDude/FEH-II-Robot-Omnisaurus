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
    //front switches
    //move forward if switches not toggled
    if(LimitFrontLeft.Value()){
        left_motor.SetPercent(speed);
    }
 
    if(LimitFrontRight.Value()){
        right_motor.SetPercent(speed);
    }

//move backwards if switches toggled
    if(!LimitFrontLeft.Value()){
        left_motor.SetPercent(-speed);
    }

    if(!LimitFrontRight.Value()){
        right_motor.SetPercent(-speed);
    }
    

    //back switches
    //move forward if switches not toggled
//     if(LimitBackLeft.Value()){
//         left_motor.SetPercent(speed);
//     }

//     if(LimitBackRight.Value()){
//         right_motor.SetPercent(speed);
//     }

// //move backwards if switches toggled
//     if(!LimitBackLeft.Value()){
//         left_motor.SetPercent(-speed);
//     }

//     if(!LimitBackRight.Value()){
//         right_motor.SetPercent(-speed);
//     }
    
    
    }
	return 0;
}
