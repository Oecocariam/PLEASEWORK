#include "main.h"
#include "file_management.cpp"
#include "block_elevator.cpp"
/**
 * A callback function for LLEMU's center button.
 *
 * When this callback is fired, it will toggle line 2 of the LCD text between
 * "I was pressed!" and nothing.
 */


int team = 1;
pros::Controller master(pros::E_CONTROLLER_MASTER);
pros::MotorGroup left_mg({-1, 2, 3});    // Creates a motor group with forwards ports 1 & 3 and reversed port 2
pros::MotorGroup right_mg({-4, 5, 6});  // Creates a motor group with forwards port 5 and reversed ports 4 & 6
pros::Motor shrimp(std::int8_t(13), pros::v5::MotorGears::green, pros::v5::MotorUnits::counts);
pros::MotorGroup krill({-11,12}, pros::v5::MotorGears::green, pros::v5::MotorUnits::counts);

pros::ADIDigitalOut MLCI1 ('A');
pros::ADIDigitalOut MLCI2 ('B');
bool piston = true;



pros::Distance intakeDistance(20);
pros::Optical color(21);

pros::Block_Elevator stimpy(shrimp, color, 55,12, team);

pros::File_management management("usd/ifYoureSeeingThisSomethingHasGoneWrong.txt", 20);

std::string data[8]= {"Time ms", "Motor1","Motor2","Motor3","Motor4","Motor5","Motor6", "Shrimp"};

double wheelCircumference = 2*3.1415*2;
double encoderUnitsPerInch = 900/wheelCircumference;
double botCircumfrance =  3.1415*8;
double wheelBase = 8;

int intakePower = 0;
int state = stimpy.getChainState();
int startTime = 0;
/**
 * hold the autonomous scripts
 * 
 */
char autonCommands[12][50][2] = {
	{{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0}},
	{{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0}},
	{{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0}},
	{{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0}},
	{{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0}},
	{{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0}},
	{{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0}},
	{{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0}},
	{{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0}},
	{{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0}},
	{{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0}},
	{{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0}},
};


double autonDoubles[256] = {
	0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0
};

int selectedAuton = 1;


/**
 *The robot drives along an arc of a given radius and arcAngle
 * 
 * \param radius
 * 	Radius of the arc Driven in inches
 * 
 * \param arcAngle
 * 	angle of the arc traveld
 * 
 * \param voltage
 * 	voltage to spin the motors out.
 * 
 */
void driveArc(double radius, double arcAngle, double voltage){

	double arcLength;
	double leftArcLength;
	double rightArcLength;

	if(arcAngle>0){
	
		arcLength = arcAngle*radius;
		leftArcLength = arcAngle*(radius-(.5*wheelBase));
		rightArcLength = arcAngle*(radius+(.5*wheelBase));

	}else{

		arcLength = arcAngle*radius;
		leftArcLength = arcAngle*(radius+(.5*wheelBase));
		rightArcLength = arcAngle*(radius-(.5*wheelBase));

	}

	double leftRatio = leftArcLength/arcLength;
	double rightRatio = rightArcLength/arcLength;
	

	left_mg.move_relative(encoderUnitsPerInch*leftArcLength, leftRatio*voltage);
    right_mg.move_relative(encoderUnitsPerInch*rightArcLength, rightRatio*voltage);		

}

/**
 * 
 * 
 */
void driveDistance(double distance, double voltage){

    left_mg.move_relative(encoderUnitsPerInch*distance, voltage);
    right_mg.move_relative(encoderUnitsPerInch*distance, voltage);
}

void powerIntake(double voltage){

	if(voltage>0){
		if(intakePower == 1){
			krill.brake();

			intakePower = 0;
		}else{
			krill.move(voltage);
			intakePower = 1;
		}
	}

	if(voltage<0){
		if(intakePower == -1){
			krill.brake();

			intakePower = 0;
		}else{
			krill.move(voltage);
			intakePower = -1;
		}
	}

};

void toggleMLCI(){

	MLCI1.set_value(!piston);
	MLCI1.set_value(!piston);

	piston = !piston;
};

bool waitType(char commandData){

	switch(commandData){

		case 'D':
			//wait for drivetrain motors to stop
			if((abs(left_mg.get_actual_velocity())<10)&&(abs(right_mg.get_actual_velocity()<10))){
				return true;
			}else{
				return false;
			}
			break;

		case 'E':
			//wait for block elevator motors to stop
			if((abs(shrimp.get_actual_velocity())<10)){
				return true;
			}else{
				return false;
			}
			break;

		case 'B':
			//wait for block elevator and drivetrain motors to stop
			if((abs(left_mg.get_actual_velocity())<10)&&(abs(right_mg.get_actual_velocity()<10))&&(abs(shrimp.get_actual_velocity())<10)){
				return true;
			}else{
				return false;
			}
			break;

	}

};




/**
 * 
 * 
 */
void turnRadians(double radians, double voltage){
    int turnDistance = radians*(.5*wheelBase);
    left_mg.move_relative(encoderUnitsPerInch*turnDistance, voltage);
    right_mg.move_relative(-encoderUnitsPerInch*turnDistance, voltage);
}

/**
 * manages which commands are being run from autonCommands during the autonomous period.
 */
void autonManager(void* jimmy){

	double setVoltage = 127;
	double setAngle = 3.1415923;
	double commandData = 0;
	
	for(int commandNumber = 0; commandNumber >= 50; commandNumber++){

		if (commandData == 0){
			commandData = autonCommands[selectedAuton][commandNumber][1];
		}

		/**
		 * This is the core command of the autonmous script.
		 * it uses two charecter integers to refer to varius commands in the autonmous library and then run them.
		 * 
		 * 
		 */
		switch(autonCommands[selectedAuton][commandNumber][0]){

			case 'D':
				//drive forward
				driveDistance(commandData, setVoltage);
				commandData = 0;
				break;

			case 'R':
				//drive along an arc
				driveArc(commandData, setAngle, setVoltage);
				commandData = 0;
				break;
			
			case 'A':
				//angle for the 'R' command
				setAngle = commandData;
				commandData = 0;
				break;
 
			case 'I':
				//toggle intake power
				powerIntake(setVoltage);
				commandData = 0;
				break;

			case 'E':
				//toggle inverted intake power
				powerIntake(-setVoltage);
				commandData = 0;
				break;

			case 'S':
				//'Score' using the block elvator
				stimpy.loadAll(setVoltage); //temp for implementation of specific number load
				commandData = 0;
				break;

			case 'T':
				//turn
				turnRadians(commandData, setVoltage);
				commandData = 0;
				break;

			case 'V':
				//set voltage to feed to future commands
				setVoltage = commandData;
				commandData = 0;
				break;

			case 'M':
				//toggle MLCI
				toggleMLCI();
				commandData = 0;
				break;

			case 'P':
				//pull a specifc double for the next command
				commandData = autonDoubles[(int)commandData];
				break;

			case 'W':
				//waits a given amount of time
				pros::delay(commandData);
				commandData = 0;
			break;

			case 'U':
				//waits for motors given to stop

				while(true){
					if (pros::Task::notify_take(true, 20)) {
						if(waitType(commandData)){
							break;
						}
					}

				}
				break;

			case 0:
				//stops the progression of the autonmous script and brakes driveTrain Motors
				left_mg.brake();
				right_mg.brake();
				while(true){
					pros::delay(1000);
				}
				break;
				

		}
		

	}
};

void autonLoop(void* jim){

	pros::Task autonManagerTask(autonManager, (void*)"Beth",(uint32_t)TASK_PRIORITY_DEFAULT, (uint16_t)TASK_STACK_DEPTH_DEFAULT, (const char*)"Auton Manager");

	while(true){

		data[0] = pros::millis() - startTime;
		data[1] = left_mg.get_voltage(0); 			   
		data[2] = left_mg.get_voltage(1); 			   
		data[3] = left_mg.get_voltage(2); 			   
		data[4] = right_mg.get_voltage(0);			   
		data[5] = right_mg.get_voltage(1);			   
		data[6] = right_mg.get_voltage(2);
		data[7] = shrimp.get_voltage();
		data[8] = state;

		autonManagerTask.notify();

		pros::delay(20);

		management.write(data);
	}

}


void on_center_button() {
	static bool pressed = false;
	pressed = !pressed;
	if (pressed) {
		pros::lcd::set_text(2, "I was pressed!");
	} else {
		pros::lcd::clear_line(2);
	}
}



std::string fileNamer = "usd/data.txt";

void toggleTeam(){
	pros::lcd::initialize();
	std::string teamType;
	if(team == 2){
		team = 1;
		teamType = "Blue";
	}else if(team ==  1){
		team = 2;
		teamType = "Red";
	}
	

	pros::lcd::set_text(3, teamType);
}

/**
 * Runs initialization code. This occurs as soon as the program is started.
 *
 * All other competition modes are blocked by initialize; it is recommended
 * to keep execution time for this mode under a few seconds.
 */
void initialize() {
	pros::lcd::initialize();
	pros::lcd::set_text(1, "Hello PROS User!");

	pros::lcd::register_btn1_cb(on_center_button);

	MLCI1.set_value(piston);
	MLCI2.set_value(piston);

	
	
	

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
void competition_initialize() {

	pros::lcd::register_btn0_cb(toggleTeam);

}

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
void autonomous() {

	int fileNumber = 0;
	startTime = pros::millis();


	while(true){

		std::string fileNamer = "usd/data_Autonomous" + std::to_string(fileNumber) + ".txt";

		char * arrFileNamer = new char [fileNamer.length()+1];
		strcpy (arrFileNamer, fileNamer.c_str());

		FILE* SD = fopen(arrFileNamer, "r");	

		if(SD == nullptr){
			break;
		}
		
		fclose(SD);

		fileNumber += 1;
	}

	pros::File_management management(fileNamer, 20);

	pros::Task autonLoopTask(autonLoop, (void*)"tod", (uint32_t)TASK_PRIORITY_DEFAULT, (uint16_t)TASK_STACK_DEPTH_DEFAULT, (const char*)"Auton Loop");


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
void opcontrol() {

	int fileNumber = 0;
	startTime = pros::millis();

	while(true){

		std::string fileNamer = "usd/data_opcontrol" + std::to_string(fileNumber) + ".txt";

		char * arrFileNamer = new char [fileNamer.length()+1];
		strcpy (arrFileNamer, fileNamer.c_str());

		FILE* SD = fopen(arrFileNamer, "r");	

		if(SD == nullptr){
			break;
		}
		
		fclose(SD);

		fileNumber += 1;
	}
	
	pros::File_management management(fileNamer, 20);

	while (true) {
		 
		management.write(data);

		int turn = master.get_analog(ANALOG_LEFT_Y);    // Gets amount forward/backward from left joystick
		int dir = master.get_analog(ANALOG_LEFT_X);  // Gets the turn left/right from right joystick
		left_mg.move(dir - turn);                      // Sets left motor voltage
		right_mg.move(dir + turn);                     // Sets right motor voltage

		int state = stimpy.getChainState();

		data[0] = pros::millis() - startTime;
		data[1] = left_mg.get_voltage(0); 
		data[2] = left_mg.get_voltage(1); 
		data[3] = left_mg.get_voltage(2); 
		data[4] = right_mg.get_voltage(0);
		data[5] = right_mg.get_voltage(1);
		data[6] = right_mg.get_voltage(2);
		data[7] = shrimp.get_voltage();
		data[8] = state;
					   

		

		pros::lcd::set_text(2, std::to_string(state));

		/**
		 *automatically loads blocks into the block elevator as they are intaked using sensor data, numerical implementation subject to change 
		 */
		if((intakeDistance.get_distance()<60)&&(abs(shrimp.get_actual_velocity())<10)){

			stimpy.hold(127);

		};

		/** 
		 * allows the block elevator to be manually loaded even if the automatic load fails, triggers by simulatanious press of A and UP on the controller
		 */
		if(master.get_digital_new_press(DIGITAL_A)&&master.get_digital_new_press(DIGITAL_UP)){

			stimpy.hold(127);

		}

		/**
		 * unloads all of the blocks from the block elevator and onto a scoring 
		 */
		if(master.get_digital_new_press(DIGITAL_Y)){

			stimpy.loadAll(127);

		}

		/**
		 *  
		 */
		if(master.get_digital_new_press(DIGITAL_B)){

			krill.move(127);
		}
 
		management.write(data);						   // write data to class

		pros::delay(20);                               // Run for 20 ms then update
		
	}
}