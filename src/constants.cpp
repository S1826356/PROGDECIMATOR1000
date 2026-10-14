#include "lemlib/api.hpp" // IWYU pragma: keep
#include "main.h"
#include "robotDecimator/constants.h"
#include "robotDecimator/functions.h"

pros::MotorGroup left_motors({-3, -5}, pros::MotorGearset::blue);
pros::MotorGroup right_motors({12, 13}, pros::MotorGearset::blue);
pros::Controller controller(pros::E_CONTROLLER_MASTER);
pros::Imu imu(14); //we don’t actually have this right now but i just have it here so i can avoid worrying about adding it later
pros::Motor rLiftMotor(20, pros::MotorGearset::green);
pros::Motor lLiftMotor(-10, pros::MotorGearset::green);
pros::adi::Port claw('B', pros::E_ADI_DIGITAL_OUT);

double dt = 0.01; //dt is in seconds, so this is 10ms
double targetPosition = 0.0;
double targetRateDegreesPerSecond = 800.0; //90 at first
double maximumTargetDegrees = 1200.0;
double kP = 45.0; //18 at firsrt
double kI = 0.08;
double kD = 1.5;
double maximumIntegral = 500.0;
double maximumVoltage = 12000.0;
