#include "main.h"
#include "lemlib/api.hpp"
#include "robotDecimator/functions.h"
#include "robotDecimator/constants.h"
#include "robotDecimator/setup.h"

#include <cstdint>

double targetRateDegreesPerSecond = 800.0; //90 at first
double maximumTargetDegrees = 1000.0;
double kP = 45.0; //18 at firsrt
double kI = 0.08;
double kD = 1.5;
double maximumIntegral = 500.0;
double maximumVoltage = 12000.0;
double dt = 0.01; //dt is in seconds, so this is 10ms
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
    //motto ganbare lillian

}