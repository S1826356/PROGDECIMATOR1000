#include "lemlib/api.hpp" // IWYU pragma: keep
#include "main.h"
#include "robotDecimator/constants.h"
#include "robotDecimator/autos.h"
#include "robotDecimator/functions.h"
#include "robotDecimator/setup.h"

void initialize(){
    pros::lcd::initialize();
    chassis.calibrate();
    pros::delay(20);
}


void opcontrol() {
    pros::lcd::initialize();
    chassis.setBrakeMode(pros::E_MOTOR_BRAKE_COAST);
    pros::delay(200);

    pros::Task driveTask(drive, nullptr, "Drive Task");
    pros::Task dr4bControlTask(dr4bControl, nullptr, "DR4B Control");
    pros::Task toggleClawTask(toggleClaw, nullptr, "Toggle Claw");

    while (true) {
        pros::delay(20);
    }
}
