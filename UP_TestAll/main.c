#include "UpLib/UP_System.h"

#ifdef DEBUG_ON
//then the program is in debug mode
#endif

int main(void) {
    UP_System_Init();
    UP_delay_ms(100);

    while (1) {
    }
}
