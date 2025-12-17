#include "main.h"
#include "string.h"
namespace pros {
    inline namespace v5 {

        class Autonomous{

            private:

                pros::MotorGroup driveLeft;
                pros::MotorGroup driveRight;

            public:


                Autonomous(pros::MotorGroup idriveLeft, pros::MotorGroup idriveRight){
                    

                    driveLeft.setEqual(idriveLeft);
                    driveRight.setEqual(idriveRight); 


                }


            
        };
    }
}