#ifndef __LC__
#define __LC__

#include <stdint.h>



enum
{
    MR_KBSR = 0xFE00, /* keyboard status */
    MR_KBDR = 0xFE02  /* keyboard data */
};



uint16_t mem_read(uint16_t address);

#endif
