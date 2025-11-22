#include "main.h"
#include "string.h"
namespace pros {
    inline namespace v5 {

        class Block_Elevator{

            private:

                pros::Motor elevatorMotor;
                std::string jimmy;
                pros::Motor* ptr = &(pros::Motor)elevatorMotor;

            public:

                Block_Elevator(pros::Motor* ielevevator){
                    
                    ptr = ielevevator;
                }
        };
    }
}