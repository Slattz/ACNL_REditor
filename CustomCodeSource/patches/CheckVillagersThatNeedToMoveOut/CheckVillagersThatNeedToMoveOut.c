#include "common.h"

void CheckVillagersThatNeedToMoveOut(u32 VillBaseOffset) {
    u32 off_status = 0x2510;
    u32 size = 0x2518;
    u32 off_byte = 0x19763;

    WRITEU8(VillBaseOffset+off_byte, 0xFF);

    for(u32 i = 0; i < 10; i++)
    {
        u32 VillOffset = VillBaseOffset+(size*i);
        u32 status = READU32(VillOffset+off_status);
        status &= ~0x603;
        WRITEU32(VillOffset+off_status, status);
    }
    
}