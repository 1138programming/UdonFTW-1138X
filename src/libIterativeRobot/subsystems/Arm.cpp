#include "main.h"
#include "libIterativeRobot/commands/Arm/StopArm.h"
#include "libIterativeRobot/commands/Arm/ArmControl.h"

const double Arm::kLowTowerPos = 2500;
const double Arm::kMidTowerPos = 3000;

Arm::Arm() {
  // Get intake motors
  leftArmMotor = Motor::getMotor(leftArmMotorPort, armMotorGearset);
  rightArmMotor = Motor::getMotor(rightArmMotorPort, armMotorGearset);
  armController = new PIDController(0.7, 0, 0);
  armController->setTolerance(25, 10);
}

void Arm::initDefaultCommand() {
  if (pros::E_CONTROLLER_DIGITAL_X > abs(20)) {
    setDefaultCommand(new ArmControl());
  }
  else
  {
    setDefaultCommand(new StopArm());
  }
  
}

/**
 * Move the arm
 * @param speed - speed of the speed arm motor
 */
void Arm::move(int speed) {
  if (bumper1->get_value() || bumper2->get_value()) {
    leftArmMotor->setSpeed(0);
    rightArmMotor->setSpeed(0);
    if (speed > 0) {
      leftArmMotor->setSpeed(speed);
      rightArmMotor->setSpeed(speed);
    } else if (speed < 0) {
      leftArmMotor->setSpeed(0);
      rightArmMotor->setSpeed(0);
    }
    leftArmMotor->resetEncoder();
    rightArmMotor->resetEncoder();
  } else {
    leftArmMotor->setSpeed(speed);
    rightArmMotor->setSpeed(speed);
  }
}

double Arm::getSensorValue() {
  return leftArmMotor->getEncoderValue();
}

void Arm::setSetpoint(double setpoint) {
  armController->setSetpoint(setpoint);
}

void Arm::lock() {
  armController->setSetpoint(getSensorValue());
}

double Arm::getSetpoint() {
  return armController->getSetpoint();
}

void Arm::calculate() {
  move((int)armController->calculate(getSensorValue()));
}

bool Arm::atSetpoint() {
  return armController->atSetpoint();
}

void Arm::resetPID() {
  armController->reset();
}
