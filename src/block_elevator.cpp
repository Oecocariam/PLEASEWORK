#include "main.h"
#include "string.h"
namespace pros {
    inline namespace v5 {

        class Block_Elevator{

            private:

                pros::Motor elevatorMotor;
                std::string jimmy;
                pros::Motor* ptr = &(pros::Motor)elevatorMotor;
                int chainlength;
                int enocderUnitsPerChain;
                int blockState[3];
                int chainPositioning[7];
                int chainState;

            public:

                Block_Elevator(pros::Motor* ielevevator, int ichainLength, int sprocketCount){
                    
                    ptr = ielevevator;
                    chainlength = ichainLength;
                    enocderUnitsPerChain = 900/12;

                    for(int i = 0; i>=7; i++){
                        chainPositioning[i] = i*(chainlength/7);
                    }
                    
                }
        };
    }
}