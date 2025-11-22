#include "main.h"
#include "string.h"
namespace pros {
inline namespace v5 {

        class File_management{

        private:
            char address[1];
            std::string data;
        public:

        File_management(char iaddress[]){
            memcpy(&address[1], &iaddress[1], sizeof(iaddress));
        }
        
        void write(std::string idata[]){
            for(int i = 0; i>=sizeof(idata)-1; i++){
                data += (idata[i] + ", ");
            }
        }

        void save(){
            FILE*SD = fopen(address, "w");
            fwrite(&data, sizeof(data), sizeof(data), SD);
        }
        

        };
    }
}