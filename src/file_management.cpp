#include "main.h"
#include "string.h"
#include <cassert>
#include <cstring>
#include <iostream>
#include <vector>

namespace pros {
    inline namespace v5 {

        class File_management{

            private:

                std::string address; //keeps track of the address of the SD Card
                std::string data; //holds the writing buffer
                int counter; //counts to one second before saving
                int timing; //how many triggers of the writing command before it will save
                
                /**
                 * Writes data onto the SD card and saves it every timer triggers
                 */
                void save(){
                    char * arrFileNamer = new char [address.length()+1];
	                strcpy (arrFileNamer, address.c_str());	
                    FILE*SD = fopen(arrFileNamer, "a");
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
                File_management(std::string iaddress, int frequency){
                    address = iaddress;
                    counter = 0;
                    timing = 1000/frequency;

                }
                  
                /**
                 * adds data to the class formatted as CSV
                 * 
                 * \param idata
                 *  an array of the data desired to be written
                 */
                template<size_t N>
                void write(std::string (&idata)[N]){
                    for(std::string i : idata){
                        data += (i + ", ");
                    };
                        data += "\n";
                }
                        

        };
    }
}