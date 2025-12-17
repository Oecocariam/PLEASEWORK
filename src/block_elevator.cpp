#include "main.h"
#include "string.h"
namespace pros {
    inline namespace v5 {

        class Block_Elevator{

            private:

                pros::Motor elevatorMotor;
                pros::Optical color;

                std::string jimmy;
                int chainlength;
                int enocderUnitsPerChain;
                int blockState[3][2];
                int chainPositioning[6];
                int chainState = 9;

            public:

                /** 
                 * Motion model block elevator software. Track the position of each 'tooth'of the chain.
                 * Tracks the position of blocks inside the block elvator with sensors
                 * 
                 * \param ielevator
                 *  motor that spins the block elevator, can use any one motor
                 * 
                 * \param icolor
                 *  optical sensor for auto color sorting.
                 * 
                 * \param ichainLength
                 *  number of chain lengths in the chain of the block elevator
                 * 
                 * \param sprocketCount
                 *  the number of teeth on the sprocket powered by the motor
                 * 
                */
                Block_Elevator(pros::Motor ielevevator, pros::Optical icolor, int ichainLength, int sprocketCount, int team){
                    
                    color.setEqual(icolor);
                    elevatorMotor.setEqual(ielevevator);
                    chainlength = ichainLength;
                    enocderUnitsPerChain = 900/sprocketCount;

                    int chainState = 9;

                    
                    chainPositioning[0] = 9;
                    chainPositioning[1] = 18;
                    chainPositioning[2] = 27;
                    chainPositioning[3] = 36;
                    chainPositioning[4] = 45;
                    chainPositioning[5] = 54;

                    blockState[0][0] = 0;
                    blockState[1][0] = 0;
                    blockState[2][0] = 0;

                    color.set_led_pwm(100);

                }

                


                /**
                 * moves a specific chain to the active position
                 * 
                 * \param chainNumber
                 *  number of chain to move to the active position
                 * \param velocity
                 *  speed to move motor at
                 */
                void chainMoveSpecific(int chainNumber, int velocity){
                    
                    int moveNumber;

                    if((chainNumber-chainState)<0){
                        moveNumber = chainlength+(chainNumber-chainState);
                    }else{
                        moveNumber = chainNumber-chainState;
                    }

                    elevatorMotor.move_relative(double(moveNumber*enocderUnitsPerChain), velocity);

                    chainState = chainNumber;

                }

                int getChainState(){
                    return chainState;
                }

                /**
                 * Sets a zero position for the chain state
                 * 
                 * \param modifier
                 *  optional: Chain state to set to (in encoder units)
                 */
                void zero(int modifier = 0){
                    chainState = modifier;
                }

                /**
                 * holds the nearest tooth to the intake to prepare for loading and labels blockk with color type, 1 is blue, 2 is red, 3 is error, implementation requires testing of hues
                 * 
                 */
                void hold(int velocity){
                
                    int nearestTooth = (chainState/9)+1;
                    if(nearestTooth >=7){
                        nearestTooth = 1;
                    }
                    
                    chainMoveSpecific(chainPositioning[nearestTooth-1], velocity);

                    

                    if(blockState[1][0]>0){
                        blockState[2][0] = blockState[1][0];
                        blockState[2][1] = nearestTooth + 2;
                    }

                    if(blockState[0][0]>0){
                        blockState[1][0] = blockState[0][0];
                        blockState[1][1] = nearestTooth + 1;
                    }

                    if((color.get_hue()<190)&&(color.get_hue()>140)){
                        blockState[0][0] = 1;
                    }else if((color.get_hue()<30)&&(color.get_hue()>20)){
                        blockState[0][0] = 2;
                    }else{
                        blockState[0][0] = 3;
                    }

                    blockState[0][1] = nearestTooth;

                }
                
                /**
                 * holds a specifc tooth to the intake to prepare for loading
                 * 
                 *  \param toothNumber;
                 */
                void hold_specific(int toothNumber, int speed){
                    
                    chainMoveSpecific(chainPositioning[toothNumber-1],speed);

                }

                /**
                 * loads the next block out of the block elevator and on to a scoring place
                 */
                void load(){
                    
                    

                    

                }

                /**
                 * loads all blocks out of the block elevator and on to a scoring place
                 */
                void loadAll(int velocity){
                    
                    int unLoadChain = 20; //chain position to move for a block to be unloaded, testing required.
                    int moveTooth;


                    if(blockState[0][0]>0){

                        moveTooth = blockState[0][1];

                    }else if(blockState[1][0]>0){

                        moveTooth = blockState[1][1];

                    }else{
                        
                        moveTooth = blockState[2][1];

                    }

                    chainMoveSpecific(chainPositioning[moveTooth] + unLoadChain, velocity);

                    while(elevatorMotor.get_current_draw() > 10){
                        pros::delay(20);
                    };

                    hold(velocity);

                }


                
        };
    }
}