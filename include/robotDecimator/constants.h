
#include "lemlib/api.hpp" // IWYU pragma: keep
#include "main.h"

extern pros::MotorGroup left_motors;
extern pros::MotorGroup right_motors;
extern pros::Controller controller;
extern pros::Imu imu;
extern pros::Motor rLiftMotor;
extern pros::Motor lLiftMotor;
extern pros::adi::Port claw;

extern double dt;
extern double targetPosition;
extern double targetRateDegreesPerSecond;
extern double maximumTargetDegrees;
extern double kP;
extern double kI;
extern double kD;
extern double maximumIntegral;
extern double maximumVoltage;