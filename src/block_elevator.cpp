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
                int chainPositioning[7];
                int chainState = 0;

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

                    for(int i = 0; i>=5; i++){
                        chainPositioning[i] = i*(chainlength/6);
                    }
                    
                }
                
                /**
                 * Moves the chain a number of chain lengths at the given velocity.
                 * 
                 * \param chainNumber
                 *  Number of chains to move the drive. 
                 * 
                 * \param velocity
                 *  Velocity of motor movement of chain
                 * 
                 */
                void chainMove(int chainNumber, int velocity){
                    
                    elevatorMotor.move_relative(enocderUnitsPerChain*chainNumber, velocity);
                    chainState += enocderUnitsPerChain*chainNumber;

                    for(int i = 0; i>=5; i++){
                        chainPositioning[i] += chainNumber;
                    }

                }


                /**
                 * 
                 * 
                 */
                void chainMoveSpecific(int chainNumber, int velocity){
                    
                    int moveNumber;

                    if((chainNumber-chainState)<0){
                        moveNumber = chainlength-(chainNumber-chainState);
                    }else{
                        moveNumber = chainNumber-chainState;
                    }

                    elevatorMotor.move_relative(moveNumber*enocderUnitsPerChain, velocity);


                    for(int i = 0; i>=5; i++){

                        if((moveNumber+chainPositioning[i])>chainNumber){
                            chainPositioning[i] = chainNumber- (chainlength-chainPositioning[i]);
                        }else{
                            chainPositioning[i] += chainNumber;
                        }


                        
                    }

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
                
                    int nearestTooth = 0;

                    for(int i = 0; i>=5; i++){
                        if(chainPositioning[i]>chainPositioning[nearestTooth]){
                            nearestTooth = i;
                        }
                    }

                    chainMoveSpecific(chainPositioning[nearestTooth], velocity);
                }
                
                /**
                 * holds a specifc tooth to the intake to prepare for loading
                 * 
                 *  \param toothNumber;
                 */
                void hold_specific(int toothNumber, int speed){
                    
                    chainMoveSpecific(chainPositioning[toothNumber],speed);

                }

                /**
                 * loads the next block into the block elevator
                 */
                void load(){
                    
                }

                


                
        };
    }
}