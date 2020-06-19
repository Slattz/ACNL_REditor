#include "../CmdKeyboardCommon.h"
#define OFF_PLAYER_INV 0x6BD0
/*
register volatile u32 *global_ptr1 asm ("lr");
register volatile u32 *global_ptr2 asm ("r1");
*/
/* DupeAll function */
void DupeAll(const wchar_t* KeyboardStr) {
    u32 playerOffset = Get_Base_Player_Address();
    if (playerOffset == 0) return;

    playerOffset+=OFF_PLAYER_INV;
    u32 itemID = READU32(playerOffset);
    for (u32 i = 0; i < 16; i++)
        WRITEU32(playerOffset+(i*4), itemID);
}