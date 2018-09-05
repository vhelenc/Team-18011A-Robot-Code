#pragma config(Motor,  port1,           dl,            tmotorVex393_HBridge, openLoop)
#pragma config(Motor,  port6,           clawMotor,     tmotorVex393_MC29, openLoop, reversed)
#pragma config(Motor,  port7,           armMotor,      tmotorVex393_MC29, openLoop, reversed)
#pragma config(Motor,  port10,          dr,            tmotorVex393_HBridge, openLoop, reversed)
//*!!Code automatically generated by 'ROBOTC' configuration wizard               !!*//

  #pragma platform(VEX)


  #pragma competitionControl(Competition)
  #pragma autonomousDuration(15)
  #pragma userControlDuration(105)


  #include "Vex_Competition_Includes.c"   //Main competition background code...do not modify!


  #define DRIVESPEED 0.75 //Ranges between 0 and 1 (changes the maximum drive speed)
  #define ARCADE true //Controls whether the drive is tank style (false) or arcade style (true)
  #define DRIVETHRESH 20 //Controls the minimum power that can be assigned to drive motors


  bool pressed = false; //this boolean describes whether the automated flip function has been triggered


  /*
  * This function assigns the arm motor to a power (ranging from -127 to 127), for as long
  * as another power is not assigned to this motor.
  */
  void assignArmMotor(int power){
  	motor[armMotor] = power;
  }


  /*
  * This function assigns the claw motor to a power (ranging from -127 to 127), for as long
  * as another power is not assigned to this motor.
  */
  void assignClawMotor(int power){
  	motor[clawMotor] = power;
  }


  /*
   * This function assigns the drive motors to powers (ranging from -127 to 127), for as long
   * other powers are not assigned to the drive motors.
   */
  void assignDriveMotors(int leftPower, int rightPower){
  	motor[dl] = leftPower;
  	motor[dr] = rightPower;
  }


  /*
  * This function takes in a drive power input and returns it as is, only if the value is
  * above a certain threshold. Otherwise, it returns 0. This function is to prevent the drive
  * from responding to slightly miscalibrated joysticks.
  */
  int filter (int input){
  	if (abs(input) > DRIVETHRESH){
  		return input;
  	}
  	return 0;
  }


  /*
  * This task controls the drive of the clawbot. It forever listens to the joystick inputs
  * to determine the power to assign to the drive motors. It is disabled when the boolean pressed
  * is set to true.
  */
  task drive(){
  	while (true){
  		if (pressed == false){
  			if (ARCADE == false){
  				motor[dl] =DRIVESPEED*((float) filter(vexRT[Ch3]));
  				motor[dr] = DRIVESPEED*((float) filter(vexRT[Ch2]));
  			}
  			else {
  				float forward = (float) filter(vexRT[Ch3]);
  				float turn = (float) filter(vexRT[Ch1]);
  				motor[dl] = DRIVESPEED*(forward+turn);
  				motor[dr] = DRIVESPEED * (forward - turn);
  			}
  		}
  	}
  }


  /*
  * This task controls the arm of the clawbot. It forever listens to the button presses
  * to determine the power to assign to the drive motors.
  */
  task arm{
  	while (true){
  		if (vexRT[Btn6U] == 1){
  			assignArmMotor(40);
  			while (vexRT[Btn6U] == 1){


  			}
  			assignArmMotor(20);
  		}
  		else if (vexRT[Btn6D] == 1){
  			assignArmMotor(-40);
  			while (vexRT[Btn6D] == 1){

  			}
  			assignArmMotor(0);
  		}
  		//Logic for button to auto-flip a gripped cap
  		else if (vexRT[Btn8L] == 1){
  			pressed = true;
  			wait1Msec(100);
  			assignDriveMotors(127, 127);
  			wait1Msec(50);
  			assignArmMotor(127);
  			wait1Msec(50);
  			assignDriveMotors(0, 0);
  			wait1Msec(400);
  			assignClawMotor(-127);
  			assignDriveMotors(127, 127);
  			wait1Msec(200);
  			assignArmMotor(0);
  			assignClawMotor(0);
  			pressed = false;
  		}
  		wait1Msec(100);
  	}
  }


  /*
  * The claw task controls the claw of the clawbot. It listens to button presses.
  */
  task claw{
  	while (true){
  		if (vexRT[Btn5U] == 1){
  			assignClawMotor(60);
  			while (vexRT[Btn5U] == 1){

  			}
  			assignClawMotor(35);
  		}
  		else if (vexRT[Btn5D] == 1){
  			assignClawMotor(-60);
  			while (vexRT[Btn5D] == 1){

  			}
  			assignClawMotor(0);
  		}
  		wait1Msec(100);
  	}
  }


  void pre_auton(){
  	//Empty method body (required to be defined for code to compile)
    //Usually, sensor and timer initialization happens here.
  }


  /*
  * The autonomous task is called when the match begins and will cut off at 15 seconds.
  * Insert the code you want your robot to run during autonomous here.
  */
  task autonomous(){
  	//Write your autonomous code here!
  	//Sample program to lift arm for 0.6 seconds and drive straight for 2 seconds
  	assignArmMotor(127);
  	wait1Msec(600);
  	assignArmMotor(25);
  	assignDriveMotors(127, 127);
  	wait1Msec(2000);
  	assignArmMotor(0);
  }


  /*
  * Main task that is called when the program starts. The job of this task is to
  * start other tasks.
  */
  task usercontrol()
  {
  	startTask(drive);
  	startTask(arm);
  	startTask(claw);
  }




//}
