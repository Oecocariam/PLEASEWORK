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

pros::Distance intakeDistance(20);
pros::Optical color(21);

pros::Block_Elevator stimpy(shrimp, color, 55,12, team);

pros::File_management management("usd/ifYoureSeeingThisSomethingHasGoneWrong.txt", 20);

std::string data[8]= {"Time ms", "Motor1","Motor2","Motor3","Motor4","Motor5","Motor6", "Shrimp"};

double wheelCircumference = 2*3.1415*2;
double encoderUnitsPerInch = 900/wheelCircumference;
double botCircumfrance =  3.1415*8;
double wheelBase = 8;

int state = stimpy.getChainState();
int startTime = 0;

/**
 * 
 * 
 */
void driveDistance(double distance, double voltage){
    while((abs(left_mg.get_actual_velocity())>10)||(abs(right_mg.get_actual_velocity()>10))){
        pros::delay(2);
    }
    left_mg.move_relative(encoderUnitsPerInch*distance, voltage);
    right_mg.move_relative(encoderUnitsPerInch*distance, voltage);
}

/**
 * 
 * 
 */
void turnRadians(double radians, double voltage){
    while((abs(left_mg.get_actual_velocity())>10)||(abs(left_mg.get_actual_velocity()>10))){
        pros::delay(2);
    }
    int turnDistance = radians*(.5*wheelBase);
    left_mg.move_relative(encoderUnitsPerInch*turnDistance, voltage);
    right_mg.move_relative(-encoderUnitsPerInch*turnDistance, voltage);
}


void autonLoop(void* jim){

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