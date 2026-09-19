#include "lemlib/api.hpp" // IWYU pragma: keep
#include "main.h"
#include "robotDecimator/constants.h"

pros::MotorGroup left_motors({-3, -5}, pros::MotorGearset::blue);
pros::MotorGroup right_motors({12, 13}, pros::MotorGearset::blue);
pros::Controller controller(pros::E_CONTROLLER_MASTER);
pros::Imu imu(14); //we don’t actually have this right now but i just have it here so i can avoid worrying about adding it later
