#include "UpLib/UP_System.h"

int main(void) {
    UP_System_Init();
    UP_delay_ms(100);

    UP_CDS_SetMode(4, CDS_SEVMODE);

    while (1) {
        UP_CDS_SetAngle(4, 100, 512);
    }
}
