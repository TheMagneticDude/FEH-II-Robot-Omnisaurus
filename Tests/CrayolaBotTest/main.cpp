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

    // while(1){
    // LCD.Clear(BLACK);
    // LCD.WriteLine("WEEE");

    // float speed = 40.0;
    // if(!LimitFrontLeft.Value() && !LimitFrontRight.Value()){
    //     LCD.WriteLine("FRONT TWO PRESSED");
    //     //if both are pressed, move backwards first
    //     left_motor.SetPercent(-speed);
    //     right_motor.SetPercent(-speed);
    //     Sleep(1000);
    //     //turn right
        
    //     left_motor.SetPercent(speed);
    //     right_motor.SetPercent(-speed);
    //     Sleep(1000);
    // }else{

    //     //front switches
    // //move forward if switches not toggled
    // if(LimitFrontLeft.Value()){
    //     left_motor.SetPercent(speed);
    // }else{
    //     LCD.WriteLine("Front Left PRESSED");
    //     //move backwards first
    //     left_motor.SetPercent(-speed);
    //     right_motor.SetPercent(-speed);
    //     Sleep(1000);
    //     //move backwards if switches toggled
    //     left_motor.SetPercent(speed);
    //     Sleep(1000);
    // }

    //     if(LimitFrontRight.Value()){
    //         right_motor.SetPercent(speed);
    //     }else{
    //         LCD.WriteLine("Front Right PRESSED");
    //         left_motor.SetPercent(-speed);
    //         right_motor.SetPercent(-speed);
    //         Sleep(1000);
    //         //move backwards if switches toggled
    //         right_motor.SetPercent(speed);
    //         Sleep(1000);
    //     }

    // }



    



    

    
    
    
    // }

    LCD.Clear(BLACK);
    LCD.WriteLine("WEEE");
//CDS Cell Test
AnalogInputPin Cds(FEHIO::P2_0);
// AnalogInputPin Cds_R(FEHIO::P1_0);
// AnalogInputPin Cds_G(FEHIO::P1_1);
// AnalogInputPin Cds_B(FEHIO::P1_2);
while(LimitFrontLeft.Value() && LimitFrontRight.Value()){
    // LCD.WriteAt("Cds_R: ",0,0);
    // LCD.WriteAt((Cds_R.Value()),0,15);
    // LCD.WriteAt("Cds_G: ",0,30);
    // LCD.WriteAt((Cds_G.Value()),0,45);
    // LCD.WriteAt("Cds_B: ",0,60);
    // LCD.WriteAt((Cds_B.Value()),0,75);
    LCD.WriteAt("Cds: ",0,0);
    LCD.WriteAt((Cds.Value()),0,15);
    if(Cds.Value() < 0.1){
        left_motor.SetPercent(40.0);
    }else{
        left_motor.SetPercent(0.0);
    }
}


    if(!LimitFrontLeft.Value() && !LimitFrontRight.Value()){
    //hardcoded
    float speed = 40.0;
    bool next = false;
    //forwards
    while(LimitFrontLeft.Value() && LimitFrontRight.Value()){
        LCD.WriteLine("Init move forwards");
        left_motor.SetPercent(speed);
        right_motor.SetPercent(speed);
    }
        LCD.WriteLine("FRONT TWO PRESSED");
        //if both are pressed, move backwards first
        left_motor.SetPercent(-speed);
        right_motor.SetPercent(-speed);
        Sleep(500);
        //turn right
        LCD.WriteLine("Turning right");
        left_motor.SetPercent(-speed);
        right_motor.SetPercent(speed);
        Sleep(700);
        LCD.WriteLine("move forwards");
        left_motor.SetPercent(speed);
        right_motor.SetPercent(speed);
        
        while(!next){
            if(!LimitFrontLeft.Value() || !LimitFrontRight.Value()){
                next = true;
            LCD.WriteLine("FRONT TWO PRESSED AGAIN");
            
                left_motor.SetPercent(-speed);
                right_motor.SetPercent(-speed);
                Sleep(500);
                LCD.WriteLine("Turning Left");
                left_motor.SetPercent(speed);
                right_motor.SetPercent(-speed);
                Sleep(700);
            }
            
        }
        next = false;
        LCD.WriteLine("move forwards");
        left_motor.SetPercent(speed);
        right_motor.SetPercent(speed);
        while(!next){
            while(!LimitFrontLeft.Value() || !LimitFrontRight.Value()){
                next = true;
            LCD.WriteLine("FRONT TWO PRESSED THIRD TIME");
                LCD.WriteLine("Turning Left");
                left_motor.SetPercent(speed);
                right_motor.SetPercent(-speed);
                Sleep(700);
            }
            
        }
        next = false;
        LCD.WriteLine("move forwards");
        left_motor.SetPercent(speed);
        right_motor.SetPercent(speed);

        if(!LimitFrontLeft.Value() || !LimitFrontRight.Value()){
        left_motor.SetPercent(0);
        right_motor.SetPercent(0);
        }

    }
    
	return 0;
}
