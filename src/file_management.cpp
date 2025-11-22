#include "main.h"
#include "string.h"
namespace pros {
    inline namespace v5 {

        class File_management{

        private:
            char address[10]; //keeps track of the address of the SD Card
            std::string data; //holds the writing buffer
            int counter; //counts to one second before saving
        public:

        /**
        * this class stores a buffer of information before writing it to the Sd card
        * 
        * \param iaddress
        *   the address for the SD card as a char array
        */
        File_management(char iaddress[]){
            memcpy(&address, &iaddress, sizeof(iaddress));
            counter = 0;
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
            if(counter>=50){
                save();
            }

        }
        /**
         * Writes data onto the SD card
         */
        void save(){
            FILE*SD = fopen(address, "w");
            fwrite(&data, sizeof(data), sizeof(data), SD);
            fclose(SD);
            data = "";
        }
        

        };
    }
}