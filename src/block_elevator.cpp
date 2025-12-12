#include "main.h"
#include "string.h"
namespace pros {
    inline namespace v5 {

        class Block_Elevator{

            private:

                pros::Motor elevatorMotor;
                std::string jimmy;
                int chainlength;
                int enocderUnitsPerChain;
                int blockState[3];
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
                 * \param ichainLength
                 *  number of chain lengths in the chain of the block elevator
                 * 
                 * \param sprocketCount
                 *  the number of teeth on the sprocket powered by the motor
                 * 
                */
                Block_Elevator(pros::Motor ielevevator, int ichainLength, int sprocketCount){
                    
                    
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

                    blockState[0] = 0;
                    blockState[1] = 0;
                    blockState[2] = 0;

                }
                


                /**
                 * 
                 * 
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
                 *  optioncal: Chain state to set to (in encoder units)
                 */
                void zero(int modifier = 0){
                    chainState = modifier;
                }

                /**
                 * holds the nearest tooth to the intake to prepare for loading
                 * 
                 */
                void hold(int velocity){
                
                    int nearestTooth = (chainState/9)+1;
                    if(nearestTooth >=7){
                        nearestTooth = 1;
                    }
                    
                    chainMoveSpecific(chainPositioning[nearestTooth-1], velocity);

                    if(blockState[1]){
                        blockState[2] =1;
                    }

                    if(blockState[0]){
                        blockState[1] =1;
                    }

                    blockState[0] = 1;
                    
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
                 * loads the next block out of the block elevator
                 */
                void load(){
                    


                }

                /**
                 * loads all blocks out of the block elevator
                 */
                void loadAll(){
                    

                }


                
        };
    }
}