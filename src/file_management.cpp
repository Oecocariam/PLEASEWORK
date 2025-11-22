#include "main.h"
#include "string.h"
namespace pros {
    inline namespace v5 {

        class File_management{

        private:

            char address[0]; //keeps track of the address of the SD Card
            std::string data; //holds the writing buffer
            int counter; //counts to one second before saving
            int timing; //how many triggers of the writing command before it will save
            
            /**
             * Writes data onto the SD card and saves it every timer triggers
             */
            void save(){
                FILE*SD = fopen(address, "w");
                fwrite(&data, sizeof(data), sizeof(data), SD);
                fclose(SD);
                data = "";
            }

        public:

            /**
            * this class stores a buffer of information before writing it to the Sd card
            * 
            * \param iaddress
            *   the address for the SD card as a char array
            * 
            * \param frequency
            *   enter the loop delay in msec for saves once a second
            */
            File_management(char iaddress[], int frequency){
                memcpy(&address, &iaddress, sizeof(iaddress));
                counter = 0;
                timing = 1000/frequency;
            }
            
            /**
             * adds data to the class formatted as CSV
             * 
             * \param idata
             *  an array of the data desired to be written
             */
            void write(std::string idata[]){
                for(int i = 0; i>=sizeof(idata)-1; i++){
                    data += (idata[i] + ", ");
                }
                data += "\n";
                if(counter>=timing){
                    save();
                }
            }

        

        };
    }
}