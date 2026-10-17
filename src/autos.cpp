#include "main.h"
#include "lemlib/api.hpp"
#include "robotDecimator/constants.h"
#include "robotDecimator/setup.h"

#include <cstdint>

double leftInt = 0.0;
double rightInt = 0.0;
double prevLeftPos = 0.0;
double prevRightPos = 0.0;
double leftPos = 0;
double rightPos = 0;

void autoLiftControl(int targetPosition){
    while(left_motors.get_position() != targetPosition && right_motors.get_position() != targetPosition){
        leftPos = lLiftMotor.get_position();
        rightPos = rLiftMotor.get_position();
        double leftError = targetPosition - leftPos;
        double rightError = targetPosition - rightPos;
        double leftVelocity = (leftPos - prevLeftPos) / dt;
        double rightVelocity = (rightPos - prevRightPos) / dt;

        double leftInt = leftInt + leftError * dt;
        double rightInt = rightInt + rightError * dt;

        double leftOutput =  kP * leftError + kI * leftInt - kD * leftVelocity;
        double rightOutput = kP * rightError + kI * rightInt - kD * rightVelocity;
        rLiftMotor.move_voltage(static_cast<std::int32_t>(rightOutput));
        lLiftMotor.move_voltage(static_cast<std::int32_t>(leftOutput));
        prevLeftPos = leftPos;
        prevRightPos = rightPos;
        pros::delay(dt*1000); //delay for 10 ms
    }
}

void matchAuton(void* param){
//LIFT GOTTA BE AT THE BOTTOM OR ELSE WE"RE RUINED!!!!
//also this zeroes the position
    rLiftMotor.tare_position();
    lLiftMotor.tare_position();
    claw_engaged = false; //close claw
    claw.set_value(claw_engaged);
    chassis.setPose(0,0,26);
    //starting is -52.6, -35 in abs coord
    autoLiftControl(100);
    chassis.moveToPoint(6.9, 14, 700);
    pros::delay(200);
    autoLiftControl(-40);
    claw_engaged = true; //open claw
    claw.set_value(claw_engaged);


    //motto ganbare lillian

}