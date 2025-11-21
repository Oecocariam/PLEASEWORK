#include "main.h"

namespace pros {
inline namespace v5 {

        class File_management{

        public:
            FILE*sdCard;

        File_management(char address[]){
            sdCard = fopen(address, "w");

        }

        };
    }
}