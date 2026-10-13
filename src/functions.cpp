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

void drive(void* param) {
    while (true) {
        double forward = controller.get_analog(pros::E_CONTROLLER_ANALOG_LEFT_Y);
        double turn = controller.get_analog(pros::E_CONTROLLER_ANALOG_RIGHT_X);
        // feed directly to chassis arcade (LEMLib handles scaling)
        chassis.arcade(forward, turn);
        pros::delay(20);
    }
    // stop chassis on exit
    left_motors.move(0);
    right_motors.move(0);
}

double safety(double value, double lower, double upper) {
    if (value < lower) return lower;
    if (value > upper) return upper;
    return value;
}

void dr4bControl(void* param) {

//LIFT GOTTA BE AT THE BOTTOM OR ELSE WE"RE RUINED!!!!
//also this zeroes the position
if (rLiftMotor.tare_position() == PROS_ERR ||
        lLiftMotor.tare_position() == PROS_ERR){
        rLiftMotor.move_voltage(0);
        lLiftMotor.move_voltage(0);
        pros::lcd::print(0, "Lift encoder zeroing failed");
        return;
    }

    targetPosition = 0.0;
    double leftInt = 0.0;
    double rightInt = 0.0;
    double prevLeftPos = 0.0;
    double prevRightPos = 0.0;
    double dt = 0.01; //dt is in seconds, so this is 10ms

    while (true) {
        int targetDirection = 0;
        /*if(controller.get_digital(pros::E_CONTROLLER_DIGITAL_L1)){
          targetDirection = 1; //its going up up up
        }
        if(controller.get_digital(pros::E_CONTROLLER_DIGITAL_R1)){
          targetDirection = -1; //its going down down down
        }*/

        std::uint8_t state = pros::lcd::read_buttons();
        if (state & LCD_BTN_LEFT) {
            targetDirection = -1; // Left button = Down
        } else if (state & LCD_BTN_RIGHT) {
            targetDirection = 1;  // Right button = Up
        } else {
            targetDirection = 0;  // No button pressed = STOP moving target
        }

        //safety net, if it goes above it returns the maximum, it it goes below it returns the minimum
        targetPosition = safety(
            targetPosition + targetDirection * targetRateDegreesPerSecond * dt,
            -60.0,
            maximumTargetDegrees
        );

        double leftPos = lLiftMotor.get_position();
        double rightPos = rLiftMotor.get_position();
        double leftError = targetPosition - leftPos;
        double rightError = targetPosition - rightPos;
        double leftVelocity = (leftPos - prevLeftPos) / dt;
        double rightVelocity = (rightPos - prevRightPos) / dt;

      //in case lift gets stuck
        double leftMayInt = safety(leftInt + leftError * dt, -maximumIntegral, maximumIntegral);
        double rightMayInt = safety(rightInt + rightError * dt, -maximumIntegral, maximumIntegral);

        double leftMayOutput = kP * leftError + kI * leftMayInt - kD * leftVelocity;
        double rightMayOutput = kP * rightError + kI * rightMayInt - kD * rightVelocity;

        if ((leftMayOutput <= maximumVoltage && leftMayOutput >= -maximumVoltage) ||
            (leftMayOutput > maximumVoltage && leftError < 0.0) ||
            (leftMayOutput < -maximumVoltage && leftError > 0.0)) {
            leftInt = leftMayInt;
        }
        if ((rightMayOutput <= maximumVoltage && rightMayOutput >= -maximumVoltage) ||
            (rightMayOutput > maximumVoltage && rightError < 0.0) ||
            (rightMayOutput < -maximumVoltage && rightError > 0.0)) {
            rightInt = rightMayInt;
        }
        //end here, the above is just in case it gets stuck and shoots up after

        double leftOutput = safety(
            kP * leftError + kI * leftInt - kD * leftVelocity,
            -maximumVoltage,
            maximumVoltage
        );
        double rightOutput = safety(
            kP * rightError + kI * rightInt - kD * rightVelocity,
            -maximumVoltage,
            maximumVoltage
        );

        rLiftMotor.move_voltage(static_cast<std::int32_t>(rightOutput));
        lLiftMotor.move_voltage(static_cast<std::int32_t>(leftOutput));

        prevLeftPos = leftPos;
        prevRightPos = rightPos;
        pros::delay(dt*1000);
        /*
        while(controller.get_digital(pros::E_CONTROLLER_DIGITAL_L1)){
            rLiftMotor.move(50);
            lLiftMotor.move(50);
        }

        while(controller.get_digital(pros::E_CONTROLLER_DIGITAL_R1)){
            rLiftMotor.move(-50);
            lLiftMotor.move(-50);
        }
        //moto gambaru lydia
        rLiftMotor.move(0);
        lLiftMotor.move(0);
        pros::delay(10);*/
}
}

void toggleClaw(void* param){
    bool claw_engaged = false;
    while(true){
        if(controller.get_digital_new_press(pros::E_CONTROLLER_DIGITAL_A)){
            pros::lcd::print(2, "i hate this");
            claw_engaged = !claw_engaged;
            claw.set_value(claw_engaged);
            pros::lcd::print(3, "Claw Engaged: %s", claw_engaged ? "true" : "false");
            pros::delay(10);
        }
        pros::delay(100);
        }
    }


    