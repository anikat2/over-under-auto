#include "main.h"

pros::Motor motor (1, pros::E_MOTOR_GEARSET_18, false);
/////
// For installation, upgrading, documentations and tutorials, check out our website!
// https://ez-robotics.github.io/EZ-Template/
/////

// Chassis constructor
ez::Drive chassis(
    // Left Chassis Ports (negative port will reverse it!)
    //   the first port is used as the sensor
    {19, -20}

    // Right Chassis Ports (negative port will reverse it!)
    //   the first port is used as the sensor
    ,
    {-5, 2}

    // IMU Port
    ,
    11

    // Wheel Diameter (Remember, 4" wheels without screw holes are actually 4.125!)
    ,
    3.25

    // Cartridge RPM
    ,
    600

    // External Gear Ratio (MUST BE DECIMAL) This is WHEEL GEAR / MOTOR GEAR
    // eg. if your drive is 84:36 where the 36t is powered, your RATIO would be 84/36 which is 2.333
    // eg. if your drive is 60:36 where the 36t is powered, your RATIO would be 60/36 which is 0.6
    // eg. if your drive is 36:60 where the 60t is powered, your RATIO would be 36/60 which is 0.6
    ,
    0.6);

/**
 * Runs initialization code. This occurs as soon as the program is started.
 *
 * All other competition modes are blocked by initialize; it is recommended
 * to keep execution time for this mode under a few seconds.
 */
void initialize()
{

  pros::delay(500); // Stop the user from doing anything while legacy ports configure

  default_constants(); // Set the drive to your own constants from autons.cpp!

  // Initialize chassis and auton selector
  chassis.initialize();
  ez::as::initialize();
  master.rumble(".");


}

/**
 * Runs while the robot is in the disabled state of Field Management System or
 * the VEX Competition Switch, following either autonomous or opcontrol. When
 * the robot is enabled, this task will exit.
 */
void disabled() {}

/**
 * Runs after initialize(), and before autonomous when connected to the Field
 * Management System or the VEX Competition Switch. This is intended for
 * competition-specific initialization routines, such as an autonomous selector
 * on the LCD.
 *
 * This task will exit when the robot is enabled and autonomous or opcontrol
 * starts.
 */
void competition_initialize() {}

/**
 * Runs the user autonomous code. This function will be started in its own task
 * with the default priority and stack size whenever the robot is enabled via
 * the Field Management System or the VEX Competition Switch in the autonomous
 * mode. Alternatively, this function may be called in initialize or opcontrol
 * for non-competition testing purposes.
 *
 * If the robot is disabled or communications is lost, the autonomous task
 * will be stopped. Re-enabling the robot will restart the task, not re-start it
 * from where it left off.
 */
void autonomous()
{
  chassis.pid_targets_reset();               // Resets PID targets to 0
  chassis.drive_imu_reset();                 // Reset gyro position to 0
  chassis.drive_sensor_reset();              // Reset drive sensors to 0
  chassis.drive_brake_set(MOTOR_BRAKE_HOLD); // Set motors to hold.  This helps autonomous consistency

  far_auton();
  //drive_example();
  //turn_example();
}

/**
 * Runs the operator control code. This function will be started in its own task
 * with the default priority and stack size whenever the robot is enabled via
 * the Field Management System or the VEX Competition Switch in the operator
 * control mode.
 *
 * If no competition control is connected, this function will run immediately
 * following initialize().
 *
 * If the robot is disabled or communications is lost, the
 * operator control task will be stopped. Re-enabling the robot will restart the
 * task, not resume it from where it left off.
 */
void opcontrol()
{
  // This is preference to what you like to drive on
  chassis.drive_brake_set(MOTOR_BRAKE_COAST);

  while (true)
  {

    // PID Tuner
    // After you find values that you're happy with, you'll have to set them in auton.cpp
    if (!pros::competition::is_connected())
    {
      // Enable / Disable PID Tuner
      //  When enabled:
      //  * use A and Y to increment / decrement the constants
      //  * use the arrow keys to navigate the constants
      if (master.get_digital_new_press(DIGITAL_X))
        chassis.pid_tuner_toggle();

      // Trigger the selected autonomous routine
      if (master.get_digital_new_press(DIGITAL_B))
        autonomous();

      chassis.pid_tuner_iterate(); // Allow PID Tuner to iterate
    }

    // chassis.opcontrol_tank(); // Tank control
    chassis.opcontrol_arcade_standard(ez::SPLIT); // Standard split arcade
    // chassis.opcontrol_arcade_standard(ez::SINGLE); // Standard single arcade
    // chassis.opcontrol_arcade_flipped(ez::SPLIT); // Flipped split arcade
    // chassis.opcontrol_arcade_flipped(ez::SINGLE); // Flipped single arcade


    pros::delay(ez::util::DELAY_TIME); // This is used for timer calculations!  Keep this ez::util::DELAY_TIME
  }
}
