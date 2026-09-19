#include "lemlib/api.hpp" // IWYU pragma: keep
#include "main.h"
#include "robotDecimator/constants.h"
#include "robotDecimator/autos.h"
#include "robotDecimator/functions.h"
#include "robotDecimator/setup.h"


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

void initialize(){
    pros::lcd::initialize();
    chassis.calibrate();
    pros::delay(20);
}


void opcontrol() {
    pros::lcd::initialize();
    chassis.setBrakeMode(pros::E_MOTOR_BRAKE_COAST);
    pros::Task driveTask(drive, NULL, "Drive Task");
    pros::delay(200);
}
