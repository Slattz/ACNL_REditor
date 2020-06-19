#include "common.h"

u32 GetNextRainbowColour(void) {
    static const u32 RainbowTable[6] = {0xFF0000FF, 0xFF7F00FF, 0xFFEF00FF, 0x00F11DFF, 0x0079FFFF, 0xA800FFFF};
    u32 off_colourstep = 0x945010;

    u8 colourstep = READU8(off_colourstep);
    if (colourstep >= 6) colourstep = 0;

    u32 colour = RainbowTable[colourstep];
    colourstep++;
    WRITEU8(off_colourstep, colourstep);
    return colour;
}